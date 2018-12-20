#ifndef RENDERER_H
#define RENDERER_H
#include<SDL2/SDL.h>
#include<vector>
#include "Transform.h"
#include "Point3.h"
#include "Shape.h"
#include <memory>
#include "PointLight.h"
#include "BackfaceCulling.h"
#include <functional>

class Renderer
{
    protected:
        using pface = std::unique_ptr<Face>; ///< just a shorthand

    private:
        SDL_Renderer* m_renderer;

        int m_screenWidth;
        int m_screenHeight;

        float m_screenZ;                ///< the Z position of the screen
        float m_projectionPointZ;       ///< the Z position of the projection point (should be grater than m_screenZ)

        std::unique_ptr<PointLight> m_light;    /// a pointer to a PointLight

        std::unique_ptr<BackfaceCulling> m_backfaceCulling; /// a pointer to a backface culling object

        std::vector<pface> m_faces;     ///< all the polygons (faces) to render

        /** \brief Projects a 3D point to a 2D point
          * \param pt the point to project
          * \param projectTo where pt is going to be projected. This is the output of this function.
          */
        void project(const Point3& pt, SDL_Point& projectTo) const;

    protected:
         /** \brief draws the passed faces sorting them as to provide a depth effect */
        virtual void renderFaces(std::vector<pface>& faces) = 0;

        /** \brief actually draws the face on the screen */
        void drawFace(const Face* face);

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
          *
          * Prepares the faces of this transform for rendering. They will be
          * actually drawn when renderToScreen is called
          * \sa renderToScreen
          */
        void render(const Transform& toRender);

        /** \brief Renders all the transforms passes to render on the screen */
        void renderToScreen();

        /** \brief Sets the current point light for this renderer
          * \param light the point light to use for this renderer
          */
        void setLight(std::unique_ptr<PointLight> light);


        /** \brief Adds backface culling capabilities to this renderer
          * \param backfaceCulling the object to use for backface culling
          */
        void setBackfaceCulling(std::unique_ptr<BackfaceCulling> backfaceCulling);

        float getProjectionPointZ() const;

        /** \brief performs an inverse projection, from 2d to 3d.
          * \param point the 2d point to inverse-project into 3d space
          * \param z the z coordinate for that point
          * \return a Point3 that would be projected into point
          * given the z position of that point.
          */
        Point3 inverseProjection(const SDL_Point& point, float z) const;

        virtual ~Renderer();
};

#endif // RENDERER_H
