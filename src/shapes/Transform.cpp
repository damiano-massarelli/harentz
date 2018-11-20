#include "Transform.h"
#include "Renderer.h"
#include <algorithm>

Transform::Transform(const std::shared_ptr<Shape>& shape) : m_shape{shape}
{

}

void Transform::setRenderer(Renderer* renderer)
{
    m_renderer = renderer;
}


void Transform::setTransformationMatrix(const Mat4& matrix)
{
    m_transformMatrix = matrix;
}

const Mat4& Transform::getTransformationMatrix() const
{
    return m_transformMatrix;
}

Mat4 Transform::getWorldTransformationMatrix() const
{
    if (m_parent == nullptr)
        return m_transformMatrix;
    else
        return m_parent->getWorldTransformationMatrix() * m_transformMatrix;
}


const std::vector<Point3> Transform::getVertWorldPositions() const
{
    std::vector<Point3> worldSpacePoints;
    // This transform might not have a shape
    if (m_shape) {
        const Mat4& worldMatrix = getWorldTransformationMatrix();
        for (const auto& pt : m_shape->getVertPositions()) {
            // Transforms and translates pt which is a local space 3d point
            worldSpacePoints.push_back(worldMatrix * pt);
        }
    }
    return worldSpacePoints;
}

void Transform::setPosition(const Point3& position)
{
    m_transformMatrix[0][3] = position.x;
    m_transformMatrix[1][3] = position.y;
    m_transformMatrix[2][3] = position.z;
}

Point3 Transform::getPosition() const {
    return getTransformationMatrix() * Point3{0.0f, 0.0f, 0.0f};
}

Point3 Transform::getWorldPosition() const
{
    return getWorldTransformationMatrix() * Point3{0.0f, 0.0f, 0.0f};
}


const std::shared_ptr<Shape>& Transform::getShape() const {
    return m_shape;
}


void Transform::setParent(Transform* parent)
{
    if (m_parent != nullptr)
        parent->removeChild(this);
    m_parent = parent;
}

void Transform::addChild(Transform* child)
{
    m_children.push_back(child);
    child->setParent(this);
}

void Transform::removeChild(Transform* child)
{
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void Transform::setColor(SDL_Color color)
{
    m_color = color;
}

const SDL_Color& Transform::getColor() const
{
    return m_color;
}

void Transform::render()
{
    if (m_renderer == nullptr)
        return;

    // This transform might not have a shape
    if (m_shape)
        m_renderer->render(*this);

    for (const auto& child : m_children)
        m_renderer->render(*child);
}

Transform::~Transform()
{

}

