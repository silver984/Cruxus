#pragma once

namespace crx {
enum class keys : int {
    none = 0, // no key pressed
    apostrophe = 39, // '
    comma = 44, // ,
    minus = 45, // -
    period = 46, // .
    slash = 47, // /
    zero = 48, // 0
    one = 49, // 1
    two = 50, // 2
    three = 51, // 3
    four = 52, // 4
    five = 53, // 5
    six = 54, // 6
    seven = 55, // 7
    eight = 56, // 8
    nine = 57, // 9
    semicolon = 59, // ;
    equal = 61, // =
    a = 65, // A | a
    b = 66, // B | b
    c = 67, // C | c
    d = 68, // D | d
    e = 69, // E | e
    f = 70, // F | f
    g = 71, // G | g
    h = 72, // H | h
    i = 73, // I | i
    j = 74, // J | j
    k = 75, // K | k
    l = 76, // L | l
    m = 77, // M | m
    n = 78, // N | n
    o = 79, // O | o
    p = 80, // P | p
    q = 81, // Q | q
    r = 82, // R | r
    s = 83, // S | s
    t = 84, // T | t
    u = 85, // U | u
    v = 86, // V | v
    w = 87, // W | w
    x = 88, // X | x
    y = 89, // Y | y
    z = 90, // Z | z
    left_bracket = 91, // [
    backslash = 92, // '\'
    right_bracket = 93, // ]
    grave = 96, // `
    space = 32, // Space
    escape = 256, // Esc
    enter = 257, // Enter
    tab = 258, // Tab
    backspace = 259, // Backspace
    insert = 260, // Ins
    delete_ = 261, // Delete
    right = 262, // Cursor right
    left = 263, // Cursor left
    down = 264, // Cursor down
    up = 265, // Cursor up
    page_up = 266, // Page up
    page_down = 267, // Page down
    home = 268, // Home
    end = 269, // End
    caps_lock = 280, // Caps lock
    scroll_lock = 281, // Scroll down
    num_lock = 282, // Num lock
    print_screen = 283, // Print screen
    pause = 284, // Pause
    f1 = 290, // F1
    f2 = 291, // F2
    f3 = 292, // F3
    f4 = 293, // F4
    f5 = 294, // F5
    f6 = 295, // F6
    f7 = 296, // F7
    f8 = 297, // F8
    f9 = 298, // F9
    f10 = 299, // F10
    f11 = 300, // F11
    f12 = 301, // F12
    left_shift = 340, // Shift left
    left_control = 341, // Control left
    left_alt = 342, // Alt left
    left_super = 343, // Super left
    right_shift = 344, // Shift right
    right_control = 345, // Control right
    right_alt = 346, // Alt right
    right_super = 347, // Super right
    kb_menu = 348, // KB menu
    kp_0 = 320, // Keypad 0
    kp_1 = 321, // Keypad 1
    kp_2 = 322, // Keypad 2
    kp_3 = 323, // Keypad 3
    kp_4 = 324, // Keypad 4
    kp_5 = 325, // Keypad 5
    kp_6 = 326, // Keypad 6
    kp_7 = 327, // Keypad 7
    kp_8 = 328, // Keypad 8
    kp_9 = 329, // Keypad 9
    kp_decimal = 330, // Keypad .
    kp_divide = 331, // Keypad /
    kp_multiply = 332, // Keypad *
    kp_subtract = 333, // Keypad -
    kp_add = 334, // Keypad +
    kp_enter = 335, // Keypad Enter
    kp_equal = 336, // Keypad =
};

constexpr const keys all_keys[] =
{
    keys::none,
    keys::apostrophe,
    keys::comma,
    keys::minus,
    keys::period,
    keys::slash,
    keys::zero,
    keys::one,
    keys::two,
    keys::three,
    keys::four,
    keys::five,
    keys::six,
    keys::seven,
    keys::eight,
    keys::nine,
    keys::semicolon,
    keys::equal,
    keys::a,
    keys::b,
    keys::c,
    keys::d,
    keys::e,
    keys::f,
    keys::g,
    keys::h,
    keys::i,
    keys::j,
    keys::k,
    keys::l,
    keys::m,
    keys::n,
    keys::o,
    keys::p,
    keys::q,
    keys::r,
    keys::s,
    keys::t,
    keys::u,
    keys::v,
    keys::w,
    keys::x,
    keys::y,
    keys::z,
    keys::left_bracket,
    keys::backslash,
    keys::right_bracket,
    keys::grave,
    keys::space,
    keys::escape,
    keys::enter,
    keys::tab,
    keys::backspace,
    keys::insert,
    keys::delete_,
    keys::right,
    keys::left,
    keys::down,
    keys::up,
    keys::page_up,
    keys::page_down,
    keys::home,
    keys::end,
    keys::caps_lock,
    keys::scroll_lock,
    keys::num_lock,
    keys::print_screen,
    keys::pause,
    keys::f1,
    keys::f2,
    keys::f3,
    keys::f4,
    keys::f5,
    keys::f6,
    keys::f7,
    keys::f8,
    keys::f9,
    keys::f10,
    keys::f11,
    keys::f12,
    keys::left_shift,
    keys::left_control,
    keys::left_alt,
    keys::left_super,
    keys::right_shift,
    keys::right_control,
    keys::right_alt,
    keys::right_super,
    keys::kb_menu,
    keys::kp_0,
    keys::kp_1,
    keys::kp_2,
    keys::kp_3,
    keys::kp_4,
    keys::kp_5,
    keys::kp_6,
    keys::kp_7,
    keys::kp_8,
    keys::kp_9,
    keys::kp_decimal,
    keys::kp_divide,
    keys::kp_multiply,
    keys::kp_subtract,
    keys::kp_add,
    keys::kp_enter,
    keys::kp_equal,
};
}