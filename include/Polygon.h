#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include "Point3.h"

class Polygon
{
    /** \brief Util class to manage polygons
      * Polygon should be added in clockwise order
      */
    private:
        std::vector<Point3*> m_vertices;

    public:
        Polygon();

        void addVertex(Point3* vertex);

        Point3 getCenter() const;

        Point3 getNormal() const;

        virtual ~Polygon();

};

#endif // POLYGON_H
