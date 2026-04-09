#pragma once

namespace slv {

struct texture {
    unsigned int id{};
    int width{};
    int height{};
    int mipmaps{};
    int format{};
};

}