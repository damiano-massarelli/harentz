#ifndef MAT3_H
#define MAT3_H
#include <array>
#include "Point3.h"

struct Mat3 {
    std::array<Point3, 3> rows;
};

Point3 operator*(const Mat3& matrix, const Point3& vec);

#endif // MAT3_H
