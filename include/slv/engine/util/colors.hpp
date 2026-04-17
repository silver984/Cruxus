#pragma once
#include <slv/types/rgb.hpp>

namespace slv {
namespace color {

constexpr slv::rgb red = slv::rgb(255, 0, 0);
constexpr slv::rgb green = slv::rgb(0, 255, 0);
constexpr slv::rgb blue = slv::rgb(0, 0, 255);
constexpr slv::rgb yellow = slv::rgb(255, 255, 0);
constexpr slv::rgb cyan = slv::rgb(0, 255, 255);
constexpr slv::rgb magenta = slv::rgb(255, 0, 255);
constexpr slv::rgb blank = slv::rgb(0, 0, 0);
constexpr slv::rgb white = slv::rgb(255, 255, 255);
constexpr slv::rgb gray = slv::rgb(128, 128, 128);
constexpr slv::rgb orange = slv::rgb(255, 165, 0);
constexpr slv::rgb pink = slv::rgb(255, 192, 203);
constexpr slv::rgb purple = slv::rgb(128, 0, 128);
constexpr slv::rgb brown = slv::rgb(165, 42, 42);

}
}