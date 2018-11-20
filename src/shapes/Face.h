#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include "Point3.h"
#include <SDL2/SDL.h>

class Face
{
    /** \brief Util class to manage polygons
      * Polygon should be added in clockwise order
      */
    private:
        std::vector<Point3> m_vertices;
        SDL_Color m_color;

    public:
        Face();

        void setColor(const SDL_Color& color);

        const SDL_Color& getColor() const;

        void addVertex(Point3 vertex);

        Point3 getCenter() const;

        Point3 getNormal() const;

        const std::vector<Point3>& getVertices() const;

        virtual ~Face();

};

#endif // POLYGON_H
