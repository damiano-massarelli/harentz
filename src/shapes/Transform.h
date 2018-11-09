#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Shape.h"
#include "Point3.h"
#include <vector>
#include "Mat4.h"
#include "AbstractRenderable.h"

class Renderer;

class Transform : public AbstractRenderable
{
    private:
        Shape m_shape;
        Mat4 m_transformMatrix;

        std::vector<Transform*> m_children;
        Transform* m_parent = nullptr;

        Renderer* m_renderer = nullptr;

        void setParent(Transform* parent);

    public:
        Transform(const Shape& shape);

        void setRenderer(Renderer* renderer);

        void addChild(Transform* child);

        void removeChild(Transform* child);

        /** \brief returns the position of this transform
          * This position is always relative to this transform.
          * if the parent is transformed this position does not change
          */
        Point3 getPosition() const;

        /** \brief returns the position of this transform
          * This position is always relative to the ancestors
          * of this transform, so if one of its ancestors is moved
          * this value changes too. */
        Point3 getWorldPosition() const;

        void setPosition(const Point3& position);

        void setTransformationMatrix(const Mat4& matrix);

        const Mat4& getTransformationMatrix() const;

        Mat4 getWorldTransformationMatrix() const;

        const Shape& getShape() const;

        const std::vector<Point3> getVertWorldPositions() const;

        virtual void render() override;

        virtual ~Transform();

};

#endif // TRANSFORM_H
