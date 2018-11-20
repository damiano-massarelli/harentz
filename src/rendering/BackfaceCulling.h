#ifndef BACKFACECULLING_H
#define BACKFACECULLING_H
#include "Point3.h"
#include <vector>
#include <functional>
#include "Face.h"

class BackfaceCulling
{
    private:
        Point3 m_playerPosition;
        Point3 m_viewDirection; ///< The direction in which the player's eyes are pointing

    public:
        /** \brief Creates an object to perform backface culling.
          * \param viewDirection The direction in which the player's eyes are pointing
          * \param playerPosition the position of an imaginary player in the scene
          */
        BackfaceCulling(const Point3& playerPosition, const Point3& viewDirection);

        /** \brief checks whether a face should be rendered or not
          * \return true if the face should be rendered, false otherwise
          */
        bool shouldRender(const Face* face);

        virtual ~BackfaceCulling();

};

#endif // BACKFACECULLING_H
