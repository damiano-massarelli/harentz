#ifndef MAT4_H
#define MAT4_H
#include <array>
#include <vector>
#include <Point3.h>

struct Mat4 {
    // std vector used to ease initialization
    std::vector<std::array<float, 4>> rows{ {1.0f, 0.0f, 0.0f, 0.0f},
                                            {0.0f, 1.0f, 0.0f, 0.0f},
                                            {0.0f, 0.0f, 1.0f, 0.0f},
                                            {0.0f, 0.0f, 0.0f, 1.0f}};

    const std::array<float, 4>& operator[](int index) const;
    std::array<float, 4>& operator[](int index);
};


/** \brief performs a matrix multiplication */
Mat4 operator*(const Mat4& m1, const Mat4& m2);

/** \brief Performs a matrix vector multiplication */
Point3 operator*(const Mat4& m, const Point3& pt);


#endif // MAT4_H
