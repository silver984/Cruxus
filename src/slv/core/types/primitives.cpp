#include <slv/core/types/primitives.hpp>
#include <cmath>

namespace slv
{
    mat3 mat3::rotation(float radians)
    {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return mat3(c, -s, 0,
                    s, c, 0,
                    0, 0, 1);
    }

    mat3 mat3::operator*(const mat3& o) const
    {
        mat3 r;

        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                r.m_[row][col] =
                    m_[row][0] * o.m_[0][col] +
                    m_[row][1] * o.m_[1][col] +
                    m_[row][2] * o.m_[2][col];
            }
        }

        return r;
    }

    vec2<float> mat3::transform_point(const vec2<float>& p) const
    {
        return vec2<float>(p.x * m_[0][0] + p.y * m_[0][1] + m_[0][2],
                           p.x * m_[1][0] + p.y * m_[1][1] + m_[1][2]);
    }
}