#pragma once

namespace slv
{
    struct texture
    {
        unsigned int id{};
        int width{};
        int height{};
        int mipmaps{};
        int format{};
    };

    /*
    struct render_texture
    {
        unsigned int id{};
        slv::texture tex;
        slv::texture depth;
    };
    */
}