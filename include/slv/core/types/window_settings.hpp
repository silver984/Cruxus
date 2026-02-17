#pragma once

namespace slv
{
    enum window_settings
    {
        VSYNC = 1 << 0,
        RESIZABLE = 1 << 1,
        START_FULLSCREEN = 1 << 2,
        BORDERLESS = 1 << 3,
        TRANSPARENT = 1 << 4
    };
}