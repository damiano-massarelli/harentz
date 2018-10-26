#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include "Point3.h"

class Face
{
    /** \brief Util class to manage polygons
      * Polygon should be added in clockwise order
      */
    private:
        std::vector<Point3*> m_vertices;

    public:
        Face();

        void addVertex(Point3* vertex);

        Point3 getCenter() const;

        Point3 getNormal() const;

        virtual ~Face();

};

#endif // POLYGON_H
