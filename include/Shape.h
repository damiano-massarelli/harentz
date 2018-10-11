#ifndef SHAPE_H
#define SHAPE_H
#include <vector>
#include "Point3.h"


class Shape
{
    private:
        std::vector<Point3> m_vertPositions;
        std::vector<int> m_quadIndices;


    public:
        Shape(const std::vector<Point3>& vertPositions, const std::vector<int>& quadsIndices);

        const std::vector<Point3>& getVertPositions() const;

        const std::vector<int>& getQuadsIndices() const;

        virtual ~Shape();

};

#endif // SHAPE_H
