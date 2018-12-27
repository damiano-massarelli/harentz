#ifndef DRAWERS_H
#define DRAWERS_H
#include <vector>
#include <SDL.h>

/** \brief fills the given polygon.
  * \param the vertices of a *closed* polygon (the last vertex must coincide with the first)
  * \param the color to use to fill the polygon
  *
  * This algorithm is based on http://alienryderflex.com/polygon_fill/
  */
void fillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& fillColor);

/** \brief draws the outline of the given polygon
  * \param the vertices of a *closed* polygon (the last vertex must coincide with the first)
  * \param the color to use to draw the outline polygon
  */
void outlineDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& outlineColor);

void outlineAndFillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color, const SDL_Color& outlineColor);

#endif // DRAWERS_H
