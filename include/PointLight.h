#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <SDL2/SDL.h>
#include <vector>
#include "Point3.h"
#include "Face.h"

class PointLight
{
    private:
        Point3 m_position; ///< Light's position
        SDL_Color m_color; ///< Light's color
        float m_ambientFactor; ///< a value in [0, 1]: 0 no ambient light, 1: full ambient light

        float getDimFactor(const Face& face);

    public:
        /** \brief Creates a new PointLight with a certain position and color.
          * \param pos the position of this light
          * \param color this light's color
          */
        PointLight(const Point3& pos, const SDL_Color& color, float ambientFactor = 0.0);

        Point3& getPosition();

        /** \brief computes and returns the color of a given face.
          * \param face the face we want the color for
          * \param faceColor the original color of the face
          * \return the color of the face when light is applied
          */
        SDL_Color getColorForFace(const Face& face, const SDL_Color& faceColor);

        virtual ~PointLight();


};

#endif // POINTLIGHT_H
