#include "Point3.h"
#include<math.h>

float dot(const Point3& pt1, const Point3& pt2)
{
    return pt1.x * pt2.x + pt1.y * pt2.y + pt1.z * pt2.z;
}

Point3 cross(const Point3& pt1, const Point3& pt2)
{
    return Point3{pt1.y*pt2.z-pt1.z*pt2.y,
                 pt1.z*pt2.x-pt1.x*pt2.z,
                 pt1.x*pt2.y-pt1.y*pt2.x};
}

Point3 normalized(const Point3& pt)
{
    float len = length(pt);
    return Point3{pt.x/len, pt.y/len, pt.z/len};
}

float distance(const Point3& pt1, const Point3& pt2)
{
    return length(pt2 - pt1);
}


Point3 operator-(const Point3& pt1, const Point3& pt2)
{
    return Point3{pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z};
}

Point3 operator+(const Point3& pt1, const Point3& pt2)
{
    return Point3{pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z};
}

float length(const Point3& pt)
{
    return static_cast<float>(sqrt(dot(pt, pt)));
}

Point3 operator*(const Point3& pt, float factor)
{
    return Point3{pt.x * factor, pt.y * factor, pt.z * factor, pt.w};
}


std::ostream& operator<<(std::ostream& out, const Point3& pt) {
    out << "(" << pt.x << ", " << pt.y << ", " << pt.z << ", " << pt.w << ")";
    return out;
}
