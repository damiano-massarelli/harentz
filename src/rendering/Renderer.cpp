#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include "Face.h"
#include <drawers.h>

Renderer::Renderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ) : m_screen{screen},
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
    const SDL_Color& fillColor = toRender.getFillColor();
    const SDL_Color& outlineColor = toRender.getOutlineColor();

    std::vector<Point3> verticesWorldPos = toRender.getVertWorldPositions();
    const std::vector<int>& quadsIndices = toRender.getShape()->getQuadsIndices();

    /* Creates the faces of the object to render */
    pface face = std::make_unique<Face>();
    for (std::vector<int>::size_type i = 0; i < quadsIndices.size(); i++) {
        face->addVertex(verticesWorldPos[quadsIndices[i]]);

        /* Each 3 vertices the face is closed */
        if (i % 4 == 3) {
            face->setFillColor(fillColor);
            face->setOutlineColor(outlineColor);

            /* Render anyway if backface culling is disabled. If it is enabled check
            whether we should render or not */
            if (!m_backfaceCulling || m_backfaceCulling->shouldRender(face.get()))
                m_faces.push_back(std::move(face));

            face = std::make_unique<Face>(); // Resets face for the following iterations
        }
    }
}

void Renderer::setLight(std::unique_ptr<PointLight> light)
{
    m_light = std::move(light);
}

void Renderer::setBackfaceCulling(std::unique_ptr<BackfaceCulling>backfaceCulling)
{
    m_backfaceCulling = std::move(backfaceCulling);
}

float Renderer::getProjectionPointZ() const
{
    return m_projectionPointZ;
}

void Renderer::renderToScreen()
{
    renderFaces(m_faces); // Simply calls an implementation specific method (this will call drawFace)
    m_faces.clear(); // Clears the faces created in the method render
}

void Renderer::addToBatch(const Face* face)
{
    SDL_Point projected;
    SDL_Color faceColor = face->getFillColor();

    if (this->m_light)
        faceColor = m_light->getColorForFace(face, faceColor);

    // The current number of vertices. /6: each vertex is represented by 6 values x, y, r, g, b, a
    unsigned short i = static_cast<unsigned short>(m_vertexBatch.size()/6);

    // Iterates through the vertices and projects them onto the screen.
    for (const auto& vertex : face->getVertices()) {
        project(vertex, projected);

        // adds the projected vertex and the color for that vertex (which is actually the color of the face "normalized")
        m_vertexBatch.insert(m_vertexBatch.end(), {projected.x/1.0f, projected.y/1.0f,
                             faceColor.r/255.0f, faceColor.g/255.0f, faceColor.b/255.0f, faceColor.a/255.0f});
    }

    // Adds the indices of the vertices in order to create two triangles
    m_indexBatch.insert(m_indexBatch.end(), {i, static_cast<unsigned short>(i+1),
                        static_cast<unsigned short>(i+2), i,
                        static_cast<unsigned short>(i+2), static_cast<unsigned short>(i+3)});
}

void Renderer::drawBatch()
{
    unsigned short numVertices = static_cast<unsigned short>(m_vertexBatch.size()/6);
    unsigned int numIndices = static_cast<unsigned int>(m_indexBatch.size());

    // Draws triangles in batches
    GPU_TriangleBatch(nullptr, m_screen, numVertices, m_vertexBatch.data(), numIndices, m_indexBatch.data(), GPU_BATCH_XY_RGBA);

    // Clears the data for already drawn batches
    m_vertexBatch.clear();
    m_indexBatch.clear();
}

Renderer::~Renderer()
{

}

