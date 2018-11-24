#include "drawers.h"
#include <algorithm>
#include <DisplayManager.h>

void fillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color) {
    // Sets the color before drawing
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int screenHeight;
    SDL_GetWindowSize(DisplayManager::getInstance()->getWindow(), nullptr, &screenHeight);

    /* Fills a polygon made of polygonPoints. Consecutive points in polygonPoints
    are assumed to be the apexes of a polygon's segment. */
    auto comparator{ [](const SDL_Point& p1, const SDL_Point& p2) {return p1.y < p2.y;} };

    // Gets the points with minimum and maximum y coord.
    int minY = std::min_element(polygonPoints.begin(), polygonPoints.end(), comparator)->y;
    int maxY = std::max_element(polygonPoints.begin(), polygonPoints.end(), comparator)->y;
    minY = std::max(0, minY); // There is no point in drawing outside the screen
    maxY = std::min(screenHeight, maxY); // ...

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
            SDL_RenderDrawLine(renderer, xIntersections[index], y, xIntersections[index+1], y);
        }

        xIntersections.clear();
    }
}

void outlineDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLines(renderer, polygonPoints.data(), polygonPoints.size());
}

void outlineAndFillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color) {
    fillDrawer(renderer, polygonPoints, color);
    outlineDrawer(renderer, polygonPoints, SDL_Color{255, 255, 255, 255});
}
