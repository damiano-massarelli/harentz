#include "Transform.h"

Transform::Transform(const Shape& shape, const Point3& pos) : m_shape{shape}, m_position{pos},
                    m_transformMatrix{Point3{1.0f, 0.0f, 0.0f}, Point3{0.0f, 1.0f, 0.0f}, Point3{0.0f, 0.0f, 1.0f}}
{

}

Point3& Transform::getPosition()
{
    return m_position;
}

void Transform::setTransformationMatrix(const Mat3& matrix)
{
    m_transformMatrix = matrix;
}


const std::vector<Point3> Transform::getVertPositionsWorld() const
{
    std::vector<Point3> worldSpacePoints;
    for (const auto& pt : m_shape.getVertPositions()) {
        // Transforms and translates pt which is a local space 3d point
        worldSpacePoints.push_back(m_position + m_transformMatrix * pt);
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

