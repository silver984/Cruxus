#include <slv/types/rgb.hpp>

namespace slv {
rgb::rgb() :
    r(0),
    g(0),
    b(0)
{}

rgb::rgb(uint8_t r_val, uint8_t g_val, uint8_t b_val) {
    r = r_val;
    g = g_val;
    b = b_val;
}

rgb rgb::operator+(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c + v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return rgb(
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    );
}

rgb rgb::operator-(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c - v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return rgb(
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    );
}

rgb rgb::operator-() const {
    return rgb(
        static_cast<uint8_t>(255 - r),
        static_cast<uint8_t>(255 - g),
        static_cast<uint8_t>(255 - b)
    );
}

rgb rgb::operator*(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c * v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return rgb(
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    );
}

rgb rgb::operator/(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c / v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return rgb(
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    );
}

rgb& rgb::operator+=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c + v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator-=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c - v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator*=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c * v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator/=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c / v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator++() {
    ++r;
    ++g;
    ++b;
    return *this;
}

rgb rgb::operator++(int) {
    rgb temp = *this;
    ++r;
    ++g;
    ++b;
    return temp;
}

rgb& rgb::operator--() {
    --r;
    --g;
    --b;
    return *this;
}

rgb rgb::operator--(int) {
    rgb temp = *this;
    --r;
    --g;
    --b;
    return temp;
}

bool rgb::operator==(rgb const& rhs) const {
    return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool rgb::operator!=(rgb const& rhs) const {
    return !(*this == rhs);
}

bool rgb::operator<=(rgb const& rhs) const {
    return r <= rhs.r && g <= rhs.g && b <= rhs.b;
}

bool rgb::operator>=(rgb const& rhs) const {
    return r >= rhs.r && g >= rhs.g && b >= rhs.b;
}

bool rgb::operator<(rgb const& rhs) const {
    return r < rhs.r && g < rhs.g && b < rhs.b;
}

bool rgb::operator>(rgb const& rhs) const {
    return r > rhs.r && g > rhs.g && b > rhs.b;
}

}