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

Mat4 Transform::getTransformationMatrix() const
{
    Mat4 translation;
    translation[0][3] = m_position.x;
    translation[1][3] = m_position.y;
    translation[2][3] = m_position.z;
    return translation * m_transformMatrix;
}

Mat4 Transform::getWorldTransformationMatrix() const
{
    if (m_parent == nullptr)
        return getTransformationMatrix();
    else
        return m_parent->getWorldTransformationMatrix() * getTransformationMatrix();
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
    m_position = position;
}

Point3 Transform::getPosition() const {
    return m_position;
}

Point3 Transform::getWorldPosition() const
{
    return getWorldTransformationMatrix() * Point3{0.0f, 0.0f, 0.0f};
}


const std::shared_ptr<Shape>& Transform::getShape() const {
    return m_shape;
}

void Transform::setShape(const std::shared_ptr<Shape>& shape)
{
    m_shape = shape;
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

