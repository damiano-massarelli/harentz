#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <SDL2/SDL.h>
#include <vector>
#include "Point3.h"
#include "Polygon.h"

class PointLight
{
    private:
        Point3 m_position; ///< Light's position
        SDL_Color m_color; ///< Light's color
        float m_ambientFactor; ///< a value in [0, 1]: 0 no ambient light, 1: full ambient light

        float getDimFactor(const Polygon& polygon);

    public:
        /** \brief Creates a new PointLight with a certain position and color.
          * \param pos the position of this light
          * \param color this light's color
          */
        PointLight(const Point3& pos, const SDL_Color& color, float ambientFactor = 0.0);

        Point3& getPosition();

        /** \brief computes and returns the color of a given polygon.
          * \param polygonVertices the vertices of the polygon in world space
          * \param polygonColor the original color of the polygon
          * \return the color of the polygon when light is applied
          */
        SDL_Color getColorForPolygon(const Polygon& polygon, const SDL_Color& polygonColor);

        virtual ~PointLight();


};

#endif // POINTLIGHT_H
