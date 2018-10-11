#ifndef POINT3_H
#define POINT3_H
#include<iostream>

struct Point3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

std::ostream& operator<<(std::ostream& out, const Point3& pt);

float dot(const Point3& pt1, const Point3& pt2);

Point3 cross(const Point3& pt1, const Point3& pt2);

Point3 normalized(const Point3& pt);

Point3 operator-(const Point3& pt1, const Point3& pt2);

Point3 operator+(const Point3& pt1, const Point3& pt2);

float length(const Point3& pt);

#endif // POINT3_H
