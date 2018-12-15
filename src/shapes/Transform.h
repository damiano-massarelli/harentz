#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Shape.h"
#include "Point3.h"
#include <vector>
#include "Mat4.h"
#include "AbstractRenderable.h"
#include <SDL2/SDL.h>
#include <memory>

class Renderer;

class Transform : public AbstractRenderable
{
    private:
        std::shared_ptr<Shape> m_shape;
        Mat4 m_transformMatrix;
        Point3 m_position;

        std::vector<Transform*> m_children;
        Transform* m_parent = nullptr;

        Renderer* m_renderer = nullptr;

        SDL_Color m_color{255, 255, 255, 255};

        void setParent(Transform* parent);

    public:
        Transform(const std::shared_ptr<Shape>& shape);

        void setRenderer(Renderer* renderer);

        Renderer* getRenderer();

        void addChild(Transform* child);

        const std::vector<Transform*>& getChildren() const {return m_children;};

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

        Mat4 getTransformationMatrix() const;

        Mat4 getWorldTransformationMatrix() const;

        const std::shared_ptr<Shape>& getShape() const;

        void setShape(const std::shared_ptr<Shape>& shape);

        const std::vector<Point3> getVertWorldPositions() const;

        SDL_Color getColor() const;

        void setColor(SDL_Color color);

        virtual void render() override;

        virtual ~Transform();

};

#endif // TRANSFORM_H
