#pragma once

#include <vector>
#include <cstdint>

namespace slv
{
    namespace win32
    {
        struct render_buffers
        {
            int width{};
            int height{};
            std::vector<uint8_t> rgba;
            std::vector<uint8_t> bgra_premult;
        };

        void init_layered_window(void* handle);
        void cleanup_layered_window();
        void convert_rgba_to_bgra(slv::win32::render_buffers& buf);
        void update_layered_window(const slv::win32::render_buffers& buf);
        slv::win32::render_buffers create_render_buffers(int width, int height);
    }
}