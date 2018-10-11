#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <SDL2/SDL.h>
#include <vector>
#include "Point3.h"
#include <functional>

class PointLight
{
    private:
        Point3 m_position;
        SDL_Color m_color;

        Point3 getPolygonCenter(const std::vector<std::reference_wrapper<Point3>>& polygonVertices);

        float getDimFactor(const std::vector<std::reference_wrapper<Point3>>& polygonVertices);

    public:
        /** \brief Creates a new PointLight with a certain position and color.
          * \param pos the position of this light
          * \param color this light's color
          */
        PointLight(const Point3& pos, const SDL_Color& color);

        Point3& getPosition();

        /** \brief computes and returns the color of a given polygon.
          * \param polygonVertices the vertices of the polygon in world space
          * \param polygonColor the original color of the polygon
          * \return the color of the polygon when light is applied
          */
        SDL_Color getColorForPolygon(const std::vector<std::reference_wrapper<Point3>>& polygonVertices, const SDL_Color& polygonColor);

        virtual ~PointLight();


};

#endif // POINTLIGHT_H
