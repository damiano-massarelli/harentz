#ifndef MAT4_H
#define MAT4_H
#include <array>
#include <vector>
#include <iostream>
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

/** \brief returns the rotation matrix for a rotation about x, y and z
  * \param x the amount of rotation around x axis
  * \param y the amount of rotation around y axis
  * \param z the amount of rotation around z axis
  * \return the rotation matrix describing that rotation */
Mat4 rotation(float x, float y, float z);

Mat4 scale(float x, float y, float z);

/** \brief performs a matrix multiplication */
Mat4 operator*(const Mat4& m1, const Mat4& m2);

/** \brief Performs a matrix vector multiplication */
Point3 operator*(const Mat4& m, const Point3& pt);

std::ostream& operator<<(std::ostream& out, const Mat4& m);


#endif // MAT4_H
