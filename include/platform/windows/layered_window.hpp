#pragma once

#include <vector>
#include <cstdint>

namespace slv
{
    struct render_buffers
    {
        int width{};
        int height{};
        std::vector<uint8_t> rgba;
        std::vector<uint8_t> bgra_premult;
    };

    namespace win32
    {
        slv::render_buffers create_render_buffers(int width, int height);
        void convert_rgba_to_bgra(slv::render_buffers& buf);
        void init_layered_window(void* handle);
        void update_layered_window(const slv::render_buffers& buf);
        void cleanup_layered_window();
    }
}