#include "Shape.h"

Shape::Shape(const std::vector<Point3>& vertPositions, const std::vector<int>& quadsIndices) : m_vertPositions{vertPositions}, m_quadIndices{quadsIndices}
{

}

const std::vector<Point3>& Shape::getVertPositions() const
{
    return m_vertPositions;
}

const std::vector<int>& Shape::getQuadsIndices() const
{
    return m_quadIndices;
}


Shape::~Shape()
{

}
