#include "BackfaceCulling.h"

BackfaceCulling::BackfaceCulling(const Point3& sightDirection) : m_sightDirection{sightDirection}
{
    //ctor
}

bool BackfaceCulling::shouldRender(const std::vector<std::reference_wrapper<Point3>>& polygonVertices)
{
    Point3 normal = cross(polygonVertices[0] - polygonVertices[1], polygonVertices[2] - polygonVertices[1]);
    return dot(m_sightDirection, normal) < 0.0f;
}


BackfaceCulling::~BackfaceCulling()
{
    //dtor
}
