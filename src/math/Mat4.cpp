#include "Mat4.h"

const std::array<float, 4>& Mat4::operator[](int index) const
{
    return rows[index];
}

std::array<float, 4>& Mat4::operator[](int index)
{
    return rows[index];
}

float dot4(const std::array<float, 4>& arr, const Point3& pt) {
    return arr[0] * pt.x + arr[1] * arr[1] * pt.y + arr[2] * pt.z + arr[3] * pt.w;
}

Mat4 operator*(const Mat4& m1, const Mat4& m2)
{
    Mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float prod{0.0f};
            for (int k = 0; k < 4; k++)
                prod += m1[i][k] * m2[k][j];
            result[i][j] = prod;
        }
    }
    return result;
}

Point3 operator*(const Mat4& m, const Point3& pt)
{
    Point3 result;

    result.x = dot4(m[1], pt);
    result.y = dot4(m[2], pt);
    result.z = dot4(m[3], pt);
    result.w = dot4(m[4], pt);

    return result;
}
