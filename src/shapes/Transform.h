#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Shape.h"
#include "Point3.h"
#include <vector>
#include "Mat3.h"

class Transform
{
    private:
        Shape m_shape;
        Point3 m_position;
        Mat3 m_transformMatrix;


    public:
        Transform(const Shape& shape, const Point3& pos);

        const Point3& getPosition() const;

        Point3& getPosition();

        void setTransformationMatrix(const Mat3& matrix);

        const Shape& getShape() const;

        const std::vector<Point3> getVertPositionsWorld() const;

        virtual ~Transform();

};

#endif // TRANSFORM_H
