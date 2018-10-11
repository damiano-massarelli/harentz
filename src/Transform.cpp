#include "Transform.h"

Transform::Transform(const Shape& shape, const Point3& pos) : m_shape{shape}, m_position{pos}
{

}

Point3& Transform::getPosition()
{
    return m_position;
}

const std::vector<Point3> Transform::getVertPositionsWorld() const
{
    std::vector<Point3> worldSpacePoints;
    for (const auto& pt : m_shape.getVertPositions()) {
        worldSpacePoints.push_back(Point3{pt.x + m_position.x, pt.y + m_position.y, pt.z + m_position.z});
    }

    return worldSpacePoints;
}

const Point3& Transform::getPosition() const {
    return m_position;
}

const Shape& Transform::getShape() const {
    return m_shape;
}

Transform::~Transform()
{

}

