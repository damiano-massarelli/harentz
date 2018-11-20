#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include "Face.h"

Renderer::Renderer(SDL_Renderer* renderer,int screenWidth, int screenHeight, float screenZ, float projectionPointZ) : m_renderer{renderer},
                                                m_screenWidth{screenWidth}, m_screenHeight{screenHeight},
                                                m_projectionPointZ{projectionPointZ} ,m_screenZ{screenZ}
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

void Renderer::fillPolygon(std::vector<SDL_Point>& polygonPoints)
{
    /* Fills a polygon made of polygonPoints. Consecutive points in polygonPoints
    are assumed to be the apexes of a polygon's segment. */
    auto comparator{ [](const SDL_Point& p1, const SDL_Point& p2) {return p1.y < p2.y;} };

    // Gets the points with minimum and maximum y coord.
    int minY = std::min_element(polygonPoints.begin(), polygonPoints.end(), comparator)->y;
    int maxY = std::max_element(polygonPoints.begin(), polygonPoints.end(), comparator)->y;
    minY = std::max(0, minY); // There is no point in drawing outside the screen
    maxY = std::min(m_screenHeight, maxY); // ...

    /* Finds the intersections among polygon's segments and a virtual horizontal line scanning the
    screen from minY and maxY */
    std::vector<int> xIntersections;
    for (int y = minY; y < maxY; ++y) {
        int j{0}; // segment starting point, i (declared in for loop) is the ending point
        for (int i = 1; i < polygonPoints.size(); ++i ) {
            const SDL_Point& minYpoint = std::min(polygonPoints[j], polygonPoints[i], comparator);
            const SDL_Point& maxYpoint = std::max(polygonPoints[j], polygonPoints[i], comparator);
            if (y >= minYpoint.y && y < maxYpoint.y) {
                double interpolatedY = static_cast<double>(y - minYpoint.y) / (maxYpoint.y - minYpoint.y);
                int intersectX = static_cast<int>(minYpoint.x + interpolatedY * (maxYpoint.x - minYpoint.x));
                xIntersections.push_back(intersectX);
            }
            j = i;
        }

        // sorts the intersections from left to right
        std::sort(xIntersections.begin(), xIntersections.end());

        // Draws a line for each couple of intersections
        for (int index = 0; index < xIntersections.size(); index = index + 2) { // index = index + 2: skip points where the polygon isn't filled
            SDL_RenderDrawLine(m_renderer, xIntersections[index], y, xIntersections[index+1], y);
        }

        xIntersections.clear();
    }
}


void Renderer::render(const Transform& toRender)
{
    const SDL_Color& color = toRender.getColor();

    std::vector<Point3> verticesWorldPos = toRender.getVertWorldPositions();
    const std::vector<int>& quadsIndices = toRender.getShape()->getQuadsIndices();

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

            face = std::make_unique<Face>();
        }
    }
}

void Renderer::renderToScreen()
{
    std::sort(m_faces.begin(), m_faces.end(), [](const auto& f1, const auto&f2) {return f1->getCenter().z > f2->getCenter().z;});

    for (const auto& f : m_faces) {
        std::vector<SDL_Point> projectedPoints;
        SDL_Point projected;
        SDL_Color faceColor = f->getColor();
        for (const auto& vertex : f->getVertices()) {
            project(vertex, projected);
            projectedPoints.push_back(projected);
        }
        projectedPoints.push_back(projectedPoints[0]);

        if (m_light)
            faceColor = m_light->getColorForFace(f.get(), faceColor);

        SDL_SetRenderDrawColor(m_renderer, faceColor.r, faceColor.g, faceColor.b, 0xFF);
        fillPolygon(projectedPoints);
    }
    // drawn faces are not needed anymore
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


Renderer::~Renderer()
{

}
