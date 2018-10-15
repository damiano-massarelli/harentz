#ifndef BACKFACECULLING_H
#define BACKFACECULLING_H
#include "Point3.h"
#include <vector>
#include <functional>


class BackfaceCulling
{
    private:
        Point3 m_sightDirection; ///< The direction in which the user's eyes are pointing

    public:
        /** \brief Creates an object to perform backface culling.
          * \param sightDirection The direction in which the user's eyes are pointing
          *
          * When the result of the dot product between a polygon's normal and sightDirection
          * is greater or equal to zero that polygon should not be rendered.
          */
        BackfaceCulling(const Point3& sightDirection);

        /** \brief checks whether a polygon should be rendered or not
          * \return true if the polygon should be rendered, false otherwise
          */
        bool shouldRender(const std::vector<std::reference_wrapper<Point3>>& polygonVertices);

        virtual ~BackfaceCulling();

};

#endif // BACKFACECULLING_H
