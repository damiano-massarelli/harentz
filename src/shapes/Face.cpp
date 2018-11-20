#include "Face.h"
#include <assert.h>

Face::Face()
{
    //ctor
}

void Face::setColor(const SDL_Color& color)
{
    m_color = color;
}


const SDL_Color& Face::getColor() const
{
    return m_color;
}


void Face::addVertex(Point3 vertex)
{
    m_vertices.push_back(vertex);
}

const std::vector<Point3>& Face::getVertices() const
{
    return m_vertices;
}


Point3 Face::getCenter() const
{
    float totX = 0.0f;
    float totY = 0.0f;
    float totZ = 0.0f;
    int numElem = m_vertices.size();
    for (const auto& pt : m_vertices) {
        totX += pt.x;
        totY += pt.y;
        totZ += pt.z;
    }

    return Point3{totX/numElem, totY/numElem, totZ/numElem};
}

Point3 Face::getNormal() const
{
    assert(m_vertices.size() >= 3);
    return cross(m_vertices[0] - m_vertices[1], m_vertices[2] - m_vertices[1]);
}


Face::~Face()
{
    //dtor
}
