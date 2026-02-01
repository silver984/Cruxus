#include <platform/windows/layered_window.hpp>
#include <windows.h>
#include <emmintrin.h>
#include <cstdint>
#include <cstddef>

namespace
{
    HWND hwnd = nullptr;
    HDC hdc_screen = nullptr;
    HDC hdc_mem = nullptr;
    HBITMAP h_bitmap = nullptr;
    void* pv_bits = nullptr;
    int cached_width = 0;
    int cached_height = 0;
}

namespace slv::win32
{
    void convert_rgba_to_bgra(slv::render_buffers& buf)
    {
        uint8_t* src = buf.rgba.data();
        uint8_t* dst = buf.bgra_premult.data();

        const size_t pixels = static_cast<size_t>(buf.width) * buf.height;
        size_t i = 0Ui64;

        // process 4 pixels (16 bytes) per iteration
        for (; i + 3 < pixels; i += 4)
        {
            // load 16 bytes (4 RGBA pixels)
            
            __m128i rgba = _mm_loadu_si128(reinterpret_cast<const __m128i*>(src));

            // separate channels

            __m128i mask_ff = _mm_set1_epi32(0xFF);
            __m128i r = _mm_and_si128(_mm_srli_epi32(rgba, 0), mask_ff);
            __m128i g = _mm_and_si128(_mm_srli_epi32(rgba, 8), mask_ff);
            __m128i b = _mm_and_si128(_mm_srli_epi32(rgba, 16), mask_ff);
            __m128i a = _mm_and_si128(_mm_srli_epi32(rgba, 24), mask_ff);

            // premultiply alpha

            __m128i ap = _mm_add_epi32(a, _mm_set1_epi32(1));
            r = _mm_srli_epi32(_mm_mullo_epi32(r, ap), 8);
            g = _mm_srli_epi32(_mm_mullo_epi32(g, ap), 8);
            b = _mm_srli_epi32(_mm_mullo_epi32(b, ap), 8);

            __m128i bgra0 = _mm_or_si128(b, _mm_slli_epi32(g, 8));
            __m128i bgra1 = _mm_or_si128(_mm_slli_epi32(r, 16), _mm_slli_epi32(a, 24));
            __m128i bgra = _mm_or_si128(bgra0, bgra1);

            _mm_storeu_si128(reinterpret_cast<__m128i*>(dst), bgra);

            src += 16;
            dst += 16;
        }

        // process remaining pixels (if width * height not multiple of 4)
        for (; i < pixels; ++i)
        {
            uint8_t r1 = src[0];
            uint8_t g1 = src[1];
            uint8_t b1 = src[2];
            uint8_t a1 = src[3];
            uint32_t ap = a1 + 1;
            dst[0] = (b1 * a1) / 255Ui8;
            dst[1] = (g1 * a1) / 255Ui8;
            dst[2] = (r1 * a1) / 255Ui8;
            dst[3] = a1;
            src += 4;
            dst += 4;
        }
    }

    void init_layered_window(void* handle)
    {
        hwnd = static_cast<HWND>(handle);
        hdc_screen = GetDC(nullptr);
        LONG ex = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, ex | WS_EX_LAYERED);
        hdc_mem = CreateCompatibleDC(hdc_screen);
    }

    void update_layered_window(const slv::render_buffers& buf)
    {
        if (!hwnd || !hdc_screen)
        {
            return;
        }

        // recreate bitmap only if needed
        if (buf.width != cached_width || buf.height != cached_height)
        {
            if (h_bitmap)
            {
                DeleteObject(h_bitmap);
                h_bitmap = nullptr;
                pv_bits = nullptr;
            }

            BITMAPV4HEADER bi{};
            bi.bV4Size = sizeof(BITMAPV4HEADER);
            bi.bV4Width = buf.width;
            bi.bV4Height = buf.height;
            bi.bV4Planes = 1;
            bi.bV4BitCount = 32;
            bi.bV4V4Compression = BI_BITFIELDS;
            ((DWORD*)&bi.bV4RedMask)[0] = 0x00FF0000;
            ((DWORD*)&bi.bV4GreenMask)[0] = 0x0000FF00;
            ((DWORD*)&bi.bV4BlueMask)[0] = 0x000000FF;
            ((DWORD*)&bi.bV4AlphaMask)[0] = 0xFF000000;

            h_bitmap = CreateDIBSection(hdc_mem, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pv_bits, nullptr, 0);
            
            if (!h_bitmap || !pv_bits)
            {
                return;
            }

            SelectObject(hdc_mem, h_bitmap);

            cached_width = buf.width;
            cached_height = buf.height;
        }

        uint8_t* dst = static_cast<uint8_t*>(pv_bits);
        const size_t total_bytes = static_cast<size_t>(buf.width) * buf.height * 4;

        // use SSE2 if available, copy otherwise

#ifdef __SSE2__
        const size_t simd_bytes = (total_bytes / 16) * 16;
        size_t i = 0;
        for (; i < simd_bytes; i += 16)
        {
            __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(buf.bgra_premult.data() + i));
            _mm_storeu_si128(reinterpret_cast<__m128i*>(dst + i), data);
        }

        for (; i < total_bytes; ++i)
        {
            dst[i] = buf.bgra_premult[i];
        }
#else
        std::memcpy(dst, buf.bgra_premult.data(), total_bytes);
#endif

        // blend bitmap onto layered window

        SIZE size = { buf.width, buf.height };
        POINT pt_src = { 0, 0 };
        RECT r;
        GetWindowRect(hwnd, &r);
        POINT pt_wnd = { r.left, r.top };
        BLENDFUNCTION blend{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

        UpdateLayeredWindow(hwnd, hdc_screen, &pt_wnd, &size, hdc_mem, &pt_src, 0, &blend, ULW_ALPHA);
    }

    slv::render_buffers create_render_buffers(int width, int height)
    {
        slv::render_buffers buf{};
        buf.width = width;
        buf.height = height;
        buf.rgba.resize(static_cast<size_t>(width) * height * 4);
        buf.bgra_premult.resize(static_cast<size_t>(width) * height * 4);
        return buf;
    }

    void cleanup_layered_window()
    {
        if (hdc_mem)
        {
            DeleteDC(hdc_mem);
            hdc_mem = nullptr;
        }

        if (h_bitmap)
        {
            DeleteObject(h_bitmap);
            h_bitmap = nullptr;
        }

        if (hdc_screen)
        {
            ReleaseDC(nullptr, hdc_screen);
            hdc_screen = nullptr;
        }

        pv_bits = nullptr;
        cached_width = 0;
        cached_height = 0;
    }
}