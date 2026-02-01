#pragma once

#include <cstdint>

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

    struct render_texture
    {
        unsigned int id{};
        texture tex;
        texture depth;
    };

    template<typename T>
    struct size
    {
        T width{};
        T height{};

        constexpr size operator+(const size& rhs) const
        {
            return { width + rhs.width, height + rhs.height };
        }

        constexpr size operator-(const size& rhs) const
        {
            return { width - rhs.width, height - rhs.height };
        }

        constexpr size operator*(const size& rhs) const
        {
            return { width * rhs.width, height * rhs.height };
        }

        constexpr size operator/(const size& rhs) const
        {
            return { width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr size operator+(U rhs) const
        {
            return { width + rhs, height + rhs };
        }

        template<typename U>
        constexpr size operator-(U rhs) const
        {
            return { width - rhs, height - rhs };
        }

        template<typename U>
        constexpr size operator*(U rhs) const
        {
            return { width * rhs, height * rhs };
        }

        template<typename U>
        constexpr size operator/(U rhs) const
        {
            return { width / rhs, height / rhs };
        }

        constexpr size& operator+=(const size& rhs)
        {
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size& operator+=(U rhs)
        {
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr size& operator-=(const size& rhs)
        {
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size& operator-=(U rhs)
        {
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const size& rhs) const
        {
            return width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const size& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const size& rhs) const
        {
            return width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const size& rhs) const
        {
            return width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const size& rhs) const
        {
            return width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const size& rhs) const
        {
            return width > rhs.width && height > rhs.height;
        }
    };

    template<typename T>
    struct rect
    {
        T x{};
        T y{};
        T width{};
        T height{};

        constexpr rect operator+(const rect& rhs) const
        {
            return { x + rhs.x, y + rhs.y, width + rhs.width, height + rhs.height };
        }

        constexpr rect operator-(const rect& rhs) const
        {
            return { x - rhs.x, y - rhs.y, width - rhs.width, height - rhs.height };
        }

        constexpr rect operator*(const rect& rhs) const
        {
            return { x * rhs.x, y * rhs.y, width * rhs.width, height * rhs.height };
        }

        constexpr rect operator/(const rect& rhs) const
        {
            return { x / rhs.x, y / rhs.y, width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr rect operator+(U rhs) const
        {
            return { x + rhs, y + rhs, width + rhs, height + rhs };
        }

        template<typename U>
        constexpr rect operator-(U rhs) const
        {
            return { x - rhs, y - rhs, width - rhs, height - rhs };
        }

        template<typename U>
        constexpr rect operator*(U rhs) const
        {
            return { x * rhs, y * rhs, width * rhs, height * rhs };
        }

        template<typename U>
        constexpr rect operator/(U rhs) const
        {
            return { x / rhs, y / rhs, width / rhs, height / rhs };
        }

        constexpr rect& operator+=(const rect& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr rect& operator-=(const rect& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const rect& rhs) const
        {
            return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const rect& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const rect& rhs) const
        {
            return x <= rhs.x && y <= rhs.y && width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const rect& rhs) const
        {
            return x >= rhs.x && y >= rhs.y && width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const rect& rhs) const
        {
            return x < rhs.x && y < rhs.y && width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const rect& rhs) const
        {
            return x > rhs.x && y > rhs.y && width > rhs.width && height > rhs.height;
        }
    };

    template<typename T>
    struct vec_2
    {
        T x{};
        T y{};

        constexpr vec_2 operator+(const vec_2& rhs) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        constexpr vec_2 operator-(const vec_2& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        constexpr vec_2 operator*(const vec_2& rhs) const
        {
            return { x * rhs.x, y * rhs.y };
        }

        constexpr vec_2 operator/(const vec_2& rhs) const
        {
            return { x / rhs.x, y / rhs.y };
        }

        template<typename U>
        constexpr vec_2 operator+(U rhs) const
        {
            return { x + rhs, y + rhs };
        }

        template<typename U>
        constexpr vec_2 operator-(U rhs) const
        {
            return { x - rhs, y - rhs };
        }

        template<typename U>
        constexpr vec_2 operator*(U rhs) const
        {
            return { x * rhs, y * rhs };
        }

        template<typename U>
        constexpr vec_2 operator/(U rhs) const
        {
            return { x / rhs, y / rhs };
        }

        constexpr vec_2& operator+=(const vec_2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec_2& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            return *this;
        }

        constexpr vec_2& operator-=(const vec_2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec_2& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        constexpr bool operator==(const vec_2& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        constexpr bool operator!=(const vec_2& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const vec_2& rhs) const
        {
            return x <= rhs.x && y <= rhs.y;
        }

        constexpr bool operator>=(const vec_2& rhs) const
        {
            return x >= rhs.x && y >= rhs.y;
        }

        constexpr bool operator<(const vec_2& rhs) const
        {
            return x < rhs.x && y < rhs.y;
        }

        constexpr bool operator>(const vec_2& rhs) const
        {
            return x > rhs.x && y > rhs.y;
        }
    };

    struct rgb
    {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
    };

    enum class key
    {
        NULL_KEY = 0, // NULL, used for no key pressed

        // alphanumeric keys
        APOSTROPHE = 39, // '
        COMMA = 44, // ,
        MINUS = 45, // -
        PERIOD = 46, // .
        SLASH = 47, // /
        ZERO = 48, // 0
        ONE = 49, // 1
        TWO = 50, // 2
        THREE = 51, // 3
        FOUR = 52, // 4
        FIVE = 53, // 5
        SIX = 54, // 6
        SEVEN = 55, // 7
        EIGHT = 56, // 8
        NINE = 57, // 9
        SEMICOLON = 59, // ;
        EQUAL = 61, // =
        A = 65, // A | a
        B = 66, // B | b
        C = 67, // C | c
        D = 68, // D | d
        E = 69, // E | e
        F = 70, // F | f
        G = 71, // G | g
        H = 72, // H | h
        I = 73, // I | i
        J = 74, // J | j
        K = 75, // K | k
        L = 76, // L | l
        M = 77, // M | m
        N = 78, // N | n
        O = 79, // O | o
        P = 80, // P | p
        Q = 81, // Q | q
        R = 82, // R | r
        S = 83, // S | s
        T = 84, // T | t
        U = 85, // U | u
        V = 86, // V | v
        W = 87, // W | w
        X = 88, // X | x
        Y = 89, // Y | y
        Z = 90, // Z | z
        LEFT_BRACKET = 91, // [
        BACKSLASH = 92, // '\'
        RIGHT_BRACKET = 93, // ]
        GRAVE = 96, // `

        // function keys
        SPACE = 32, // Space
        ESCAPE = 256, // Esc
        ENTER = 257, // Enter
        TAB = 258, // Tab
        BACKSPACE = 259, // Backspace
        INSERT = 260, // Ins
        DELETE = 261, // Del
        RIGHT = 262, // Cursor right
        LEFT = 263, // Cursor left
        DOWN = 264, // Cursor down
        UP = 265, // Cursor up
        PAGE_UP = 266, // Page up
        PAGE_DOWN = 267, // Page down
        HOME = 268, // Home
        END = 269, // End
        CAPS_LOCK = 280, // Caps lock
        SCROLL_LOCK = 281, // Scroll down
        NUM_LOCK = 282, // Num lock
        PRINT_SCREEN = 283, // Print screen
        PAUSE = 284, // Pause
        F1 = 290, // F1
        F2 = 291, // F2
        F3 = 292, // F3
        F4 = 293, // F4
        F5 = 294, // F5
        F6 = 295, // F6
        F7 = 296, // F7
        F8 = 297, // F8
        F9 = 298, // F9
        F10 = 299, // F10
        F11 = 300, // F11
        F12 = 301, // F12
        LEFT_SHIFT = 340, // Shift left
        LEFT_CONTROL = 341, // Control left
        LEFT_ALT = 342, // Alt left
        LEFT_SUPER = 343, // Super left
        RIGHT_SHIFT = 344, // Shift right
        RIGHT_CONTROL = 345, // Control right
        RIGHT_ALT = 346, // Alt right
        RIGHT_SUPER = 347, // Super right
        KB_MENU = 348, // KB menu

        // keypad keys
        KP_0 = 320, // Keypad 0
        KP_1 = 321, // Keypad 1
        KP_2 = 322, // Keypad 2
        KP_3 = 323, // Keypad 3
        KP_4 = 324, // Keypad 4
        KP_5 = 325, // Keypad 5
        KP_6 = 326, // Keypad 6
        KP_7 = 327, // Keypad 7
        KP_8 = 328, // Keypad 8
        KP_9 = 329, // Keypad 9
        KP_DECIMAL = 330, // Keypad .
        KP_DIVIDE = 331, // Keypad /
        KP_MULTIPLY = 332, // Keypad *
        KP_SUBTRACT = 333, // Keypad -
        KP_ADD = 334, // Keypad +
        KP_ENTER = 335, // Keypad Enter
        KP_EQUAL = 336, // Keypad =

        // Android key buttons
        BACK = 4, // Android back button
        MENU = 5, // Android menu button
        VOLUME_UP = 24, // Android volume up button
        VOLUME_DOWN = 25 // Android volume down button
    };

    constexpr const key all_keys[] =
    {
        key::NULL_KEY,

        // alphanumeric keys
        key::APOSTROPHE,
        key::COMMA,
        key::MINUS,
        key::PERIOD,
        key::SLASH,
        key::ZERO,
        key::ONE,
        key::TWO,
        key::THREE,
        key::FOUR,
        key::FIVE,
        key::SIX,
        key::SEVEN,
        key::EIGHT,
        key::NINE,
        key::SEMICOLON,
        key::EQUAL,
        key::A,
        key::B,
        key::C,
        key::D,
        key::E,
        key::F,
        key::G,
        key::H,
        key::I,
        key::J,
        key::K,
        key::L,
        key::M,
        key::N,
        key::O,
        key::P,
        key::Q,
        key::R,
        key::S,
        key::T,
        key::U,
        key::V,
        key::W,
        key::X,
        key::Y,
        key::Z,
        key::LEFT_BRACKET,
        key::BACKSLASH,
        key::RIGHT_BRACKET,
        key::GRAVE,

        // function keys
        key::SPACE,
        key::ESCAPE,
        key::ENTER,
        key::TAB,
        key::BACKSPACE,
        key::INSERT,
        key::DELETE,
        key::RIGHT,
        key::LEFT,
        key::DOWN,
        key::UP,
        key::PAGE_UP,
        key::PAGE_DOWN,
        key::HOME,
        key::END,
        key::CAPS_LOCK,
        key::SCROLL_LOCK,
        key::NUM_LOCK,
        key::PRINT_SCREEN,
        key::PAUSE,
        key::F1,
        key::F2,
        key::F3,
        key::F4,
        key::F5,
        key::F6,
        key::F7,
        key::F8,
        key::F9,
        key::F10,
        key::F11,
        key::F12,
        key::LEFT_SHIFT,
        key::LEFT_CONTROL,
        key::LEFT_ALT,
        key::LEFT_SUPER,
        key::RIGHT_SHIFT,
        key::RIGHT_CONTROL,
        key::RIGHT_ALT,
        key::RIGHT_SUPER,
        key::KB_MENU,

        // keypad keys
        key::KP_0,
        key::KP_1,
        key::KP_2,
        key::KP_3,
        key::KP_4,
        key::KP_5,
        key::KP_6,
        key::KP_7,
        key::KP_8,
        key::KP_9,
        key::KP_DECIMAL,
        key::KP_DIVIDE,
        key::KP_MULTIPLY,
        key::KP_SUBTRACT,
        key::KP_ADD,
        key::KP_ENTER,
        key::KP_EQUAL,

        // Android key buttons
        key::BACK,
        key::MENU,
        key::VOLUME_UP,
        key::VOLUME_DOWN
    };
}