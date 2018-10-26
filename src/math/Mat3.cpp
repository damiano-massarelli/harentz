#include "Mat3.h"

Point3 operator*(const Mat3& matrix, const Point3& vec)
{
    return Point3{dot(matrix.rows[0], vec), dot(matrix.rows[1], vec), dot(matrix.rows[2], vec)};
}
