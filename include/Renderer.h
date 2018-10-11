#ifndef RENDERER_H
#define RENDERER_H
#include<SDL2/SDL.h>
#include<vector>
#include "Transform.h"
#include "Point3.h"
#include "Shape.h"
#include <memory>
#include "PointLight.h"

class Renderer
{
    private:
        SDL_Renderer* m_renderer;

        int m_screenWidth;
        int m_screenHeight;

        float m_screenZ;                ///< the Z position of the screen
        float m_projectionPointZ;       ///< the Z position of the projection point (should be grater than m_screenZ)

        std::unique_ptr<PointLight> m_light;

        /** \brief Projects a 3D point to a 2D point
          * \param pt the point to project
          * \param projectTo where pt is going to be projected. This is the output of this function.
          */
        void project(const Point3& pt, SDL_Point& projectTo) const;

        /** \brief Fills a polygon with a given color
          * \param polygonPoints 2D points representing the vertices of the polygon.
          *
          * The polygon must be closed. That is, polygonPoints[0] == polygonPoints[polygonPoints.size() - 1]
          */
        void fillPolygon(std::vector<SDL_Point>& polygonPoints);

    public:
        /** \brief Creates a new renderer
          * \param renderer an SDL2 renderer which will be used to put graphics on the screen
          * \param screenWidth the screen width
          * \param screenHeight the screen height
          * \param screenZ the z position of the virtual screen
          * \param projectionPointZ z coordinate of the projection point. This is the highest value for a point's z coord.
          */
        Renderer(SDL_Renderer* renderer, int screenWidth, int screenHeight, float screenZ, float projectionPointZ);

        /** \brief Renders a Tranform object to the screen.
          * \param toRender the transform that will be rendered
          */
        void render(const Transform& toRender);

        void setLight(std::unique_ptr<PointLight> light);

        virtual ~Renderer();


};

#endif // RENDERER_H
