#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include "Face.h"
#include "BSPTree.h"

Renderer::Renderer(SDL_Renderer* renderer,int screenWidth, int screenHeight, float screenZ, float projectionPointZ) : m_renderer{renderer},
                                                m_screenWidth{screenWidth}, m_screenHeight{screenHeight},
                                                m_projectionPointZ{projectionPointZ}, m_screenZ{screenZ}
{

}

void Renderer::project(const Point3& pt, SDL_Point& projectTo) const
{
    // perspective scaling factor
    float u = (m_projectionPointZ - m_screenZ) / (m_projectionPointZ - pt.z);
    float x = pt.x * u;
    float y = pt.y * u;

    // centers the projected points
    projectTo.x = static_cast<int>(x + m_screenWidth/2);
    projectTo.y = static_cast<int>(y + m_screenHeight/2);
}


Point3 Renderer::inverseProjection(const SDL_Point& point, float z) const
{
    float u = (m_projectionPointZ - m_screenZ) / (m_projectionPointZ - z);
    return Point3{point.x / u, point.y / u, z};
}

void Renderer::render(const Transform& toRender)
{
    const SDL_Color& color = toRender.getColor();

    std::vector<Point3> verticesWorldPos = toRender.getVertWorldPositions();
    const std::vector<int>& quadsIndices = toRender.getShape()->getQuadsIndices();

    /* Creates the faces of the object to render */
    pface face = std::make_unique<Face>();
    for (std::vector<int>::size_type i = 0; i < quadsIndices.size(); i++) {
        face->addVertex(verticesWorldPos[quadsIndices[i]]);

        /* Each 3 vertices the face is closed */
        if (i % 4 == 3) {
            face->setColor(color);

            /* Render anyway if backface culling is disabled. If it is enabled check
            whether we should render or not */
            if (!m_backfaceCulling || m_backfaceCulling->shouldRender(face.get()))
                m_faces.push_back(std::move(face));

            face = std::make_unique<Face>(); // Resets face for the following iterations
        }
    }
}

void Renderer::renderToScreen()
{
    BSPTree renderTree{m_faces};

    // Walks through the bsptree and projects the faces
    renderTree.walk(Point3{0,0, m_projectionPointZ}, [this](Face* f) {
        std::vector<SDL_Point> projectedPoints;
        SDL_Point projected;
        SDL_Color faceColor = f->getColor();
        // Iterates through the vertices and projects them onto the screen.
        for (const auto& vertex : f->getVertices()) {
            project(vertex, projected);
            projectedPoints.push_back(projected);
        }
        // A face has only three edges (despite its 4 vertices). The last edge from the last vertex to the first one is added here
        projectedPoints.push_back(projectedPoints[0]);
        if (this->m_light)
            faceColor = m_light->getColorForFace(f, faceColor);

        // If a drawer is not defined for this renderer it is not possible to draw
        if (this->m_drawer)
            m_drawer(m_renderer, projectedPoints, faceColor);
    });
    // drawn faces are not needed anymore */
    m_faces.clear();
}


void Renderer::setLight(std::unique_ptr<PointLight> light)
{
    m_light = std::move(light);
}

void Renderer::setBackfaceCulling(std::unique_ptr<BackfaceCulling>backfaceCulling)
{
    m_backfaceCulling = std::move(backfaceCulling);
}

void Renderer::setDrawer(const std::function<void(SDL_Renderer*, const std::vector<SDL_Point>&, const SDL_Color&)>& drawer)
{
    m_drawer = drawer;
}


Renderer::~Renderer()
{

}
