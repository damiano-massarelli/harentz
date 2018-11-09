#include "Face.h"
#include <assert.h>

Face::Face()
{
    //ctor
}

void Face::addVertex(Point3* vertex)
{
    m_vertices.push_back(vertex);
}


Point3 Face::getCenter() const
{
    float totX = 0.0f;
    float totY = 0.0f;
    float totZ = 0.0f;
    int numElem = m_vertices.size();
    for (auto pt : m_vertices) {
        totX += pt->x;
        totY += pt->y;
        totZ += pt->z;
    }

    return Point3{totX/numElem, totY/numElem, totZ/numElem};
}

Point3 Face::getNormal() const
{
    assert(m_vertices.size() >= 3);
    return cross(*m_vertices[0] - *m_vertices[1], *m_vertices[2] - *m_vertices[1]);
}


Face::~Face()
{
    //dtor
}