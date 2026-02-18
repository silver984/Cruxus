#pragma once

namespace slv
{
    enum window_settings
    {
        NONE = 0,
        VSYNC = 1 << 0,
        UNRESIZABLE = 1 << 1,
        START_FULLSCREEN = 1 << 2,
        BORDERLESS = 1 << 3,
        TRANSPARENT = 1 << 4
    };
}