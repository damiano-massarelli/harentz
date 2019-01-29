#ifndef POINT3_H
#define POINT3_H
#include<iostream>

/** \brief a Point3 represents a point in 3D space.
  * it also supports homogeneous coordinates through
  * its w property. Note: this property is not taken
  * into account in any of the operations of its interface */
struct Point3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float w = 1.0f;
};

std::ostream& operator<<(std::ostream& out, const Point3& pt);

float dot(const Point3& pt1, const Point3& pt2);

Point3 cross(const Point3& pt1, const Point3& pt2);

Point3 normalized(const Point3& pt);

float distance(const Point3& pt1, const Point3& pt2);

Point3 operator-(const Point3& pt1, const Point3& pt2);

Point3 operator+(const Point3& pt1, const Point3& pt2);

Point3 operator+=(Point3& pt1, const Point3& pt2);

Point3 operator*(const Point3& pt, float factor);

float length(const Point3& pt);

#endif // POINT3_H
