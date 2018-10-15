#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include "Polygon.h"

Renderer::Renderer(SDL_Renderer* renderer,int screenWidth, int screenHeight, float screenZ, float projectionPointZ) : m_renderer{renderer},
                                                m_screenWidth{screenWidth}, m_screenHeight{screenHeight},
                                                m_projectionPointZ{projectionPointZ} ,m_screenZ{screenZ}
{

}

void Renderer::project(const Point3& pt, SDL_Point& projectTo) const
{
    // This is the littlest z value. Points beyond this point won't be rendered.
    if (pt.z <= m_projectionPointZ) {
        projectTo.x = 0;
        projectTo.y = 0;
        return;
    }
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
    std::vector<Point3> verticesWorldPos = toRender.getVertPositionsWorld();
    const std::vector<int>& quadsIndices = toRender.getShape().getQuadsIndices();

    SDL_Point projected;
    std::vector<SDL_Point> projectedPolygonVertices;
    Polygon polygon;
    for (std::vector<int>::size_type i = 0; i < quadsIndices.size(); i++) {
        // projects the i-th point an puts it in projectedPolygonVertices
        project(verticesWorldPos[quadsIndices[i]], projected);
        projectedPolygonVertices.push_back(projected);

        polygon.addVertex(&verticesWorldPos[quadsIndices[i]]);

        /* Each 3 vertices the polygon is closed and drawn on the screen */
        if (i % 4 == 3) {

            /* Render anyway if backface culling is disabled. If it is enabled check
            whether we should render or not */
            //if (!m_backfaceCulling || m_backfaceCulling->shouldRender(polygonVertices)) {
                projectedPolygonVertices.push_back(projectedPolygonVertices[0]); // completes the polygon
                SDL_Color polygonColor{0xFF, 0xFF, 0xFF, 0xFF};
                if (m_light) {
                    polygonColor = m_light->getColorForPolygon(polygon, polygonColor);
                }

                SDL_SetRenderDrawColor(m_renderer ,polygonColor.r, polygonColor.g, polygonColor.b, 0xFF);
                fillPolygon(projectedPolygonVertices);
            //}

            // Free info about just drawn polygon
            projectedPolygonVertices.clear();
            polygon = Polygon();
        }
        /*SDL_SetRenderDrawColor(m_renderer , 0xFF, 0x00, 0x00, 0xFF);
        SDL_Rect pt{projected.x, projected.y, 5, 5};
        SDL_RenderFillRect(m_renderer, &pt); */
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


Renderer::~Renderer()
{

}
