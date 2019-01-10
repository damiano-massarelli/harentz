#include "Renderer.h"
#include "Face.h"
#include <algorithm>
#include <utility>
#include <algorithm>

Renderer::Renderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ) : m_screen{screen},
                                                m_screenWidth{screenWidth}, m_screenHeight{screenHeight},
                                                m_projectionPointZ{projectionPointZ}, m_screenZ{screenZ}
{

}

void Renderer::project(const Point3& pt, float& outX, float& outY) const
{
    // perspective scaling factor
    float u = (m_projectionPointZ - m_screenZ) / (m_projectionPointZ - pt.z);
    float x = pt.x * u;
    float y = pt.y * u;

    // centers the projected points
    outX = x + m_screenWidth/2;
    outY = y + m_screenHeight/2;
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
    std::vector<std::pair<float, float>> projectedPoints; // 2d projected points: used to draw outline
    SDL_Color faceColor = face->getFillColor();

    if (this->m_light)
        faceColor = m_light->getColorForFace(face, faceColor);

    // The current number of vertices. /6: each vertex is represented by 6 values x, y, r, g, b, a
    unsigned short firstVertIndex = static_cast<unsigned short>(m_vertexBatch.size()/6);

    // Iterates through the vertices and projects them onto the screen.
    float projectedX = 0;
    float projectedY = 0;
    for (const auto& vertex : face->getVertices()) {
        project(vertex, projectedX, projectedY);
        projectedPoints.push_back(std::make_pair(projectedX, projectedY));

        // adds the projected vertex and the color for that vertex (which is actually the color of the face "normalized")
        m_vertexBatch.insert(m_vertexBatch.end(), {projectedX, projectedY,
                             faceColor.r/255.0f, faceColor.g/255.0f, faceColor.b/255.0f, faceColor.a/255.0f});
    }

    // Adds the indices of the vertices in order to create two triangles
    m_indexBatch.insert(m_indexBatch.end(), {
                        firstVertIndex, static_cast<unsigned short>(firstVertIndex+1), static_cast<unsigned short>(firstVertIndex+2), // First tri
                        firstVertIndex, static_cast<unsigned short>(firstVertIndex+2), static_cast<unsigned short>(firstVertIndex+3)}); // Second tri

    addOutlineToBatch(projectedPoints, face);
}

void Renderer::addOutlineToBatch(const std::vector<std::pair<float, float>>& verts, const Face* face)
{
    const SDL_Color& color = face->getOutlineColor();
    if (color.a == 0) return; // do not draw

    const float lineWidth = 1.0f;

    float r = color.r/255.0f;
    float g = color.g/255.0f;
    float b = color.b/255.0f;
    float a = color.a/255.0f;

    const std::vector<bool>& splitVert = face->getSplitVert();

    for (std::size_t i = 0; i < verts.size(); ++i) {
        // The current number of vertices. /6: each vertex is represented by 6 values x, y, r, g, b, a
        unsigned short firstVertIndex = static_cast<unsigned short>(m_vertexBatch.size()/6);

        const auto& start = verts[i];
        const auto& end = verts[(i+1) % verts.size()];

        /* Do not render an edge if it splits a face */
        if (splitVert[i] && splitVert[(i+1) % verts.size()])
            continue;

        // What follows are the components of a *2D* vector representing the line (its direction is the direction of the line)
        Point3 lineVec{start.first - end.first, start.second - end.second, 0, 0};

        // What follows are the component of a *2D* vector that is perpendicular to the lineVec one.
        Point3 normVec = normalized(Point3{-lineVec.y, lineVec.x, 0, 0});

        // Specifies how much to move from start and end to create the vertices of the triangles
        Point3 moveVec = normVec * lineWidth;
        m_vertexBatch.insert(m_vertexBatch.end(), {start.first - moveVec.x / 2, start.second - moveVec.y / 2, r, g, b, a});
        m_vertexBatch.insert(m_vertexBatch.end(), {start.first + moveVec.x / 2, start.second + moveVec.y / 2, r, g, b, a});

        m_vertexBatch.insert(m_vertexBatch.end(), {end.first - moveVec.x / 2, end.second - moveVec.y / 2, r, g, b, a});
        m_vertexBatch.insert(m_vertexBatch.end(), {end.first + moveVec.x / 2, end.second + moveVec.y / 2, r, g, b, a});

        // Creates a line using two triangles
        m_indexBatch.insert(m_indexBatch.end(), {firstVertIndex, static_cast<unsigned short>(firstVertIndex+1),  static_cast<unsigned short>(firstVertIndex+2), // first tri
                             static_cast<unsigned short>(firstVertIndex+2),  static_cast<unsigned short>(firstVertIndex+1),  static_cast<unsigned short>(firstVertIndex+3)});
    }
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

