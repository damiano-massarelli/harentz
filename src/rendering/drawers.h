#ifndef DRAWERS_H
#define DRAWERS_H
#include <vector>
#include <SDL2/SDL.h>

/** \brief fills the given polygon.
  * \param the vertices of a *closed* polygon (the first vertex must coincide with the first)
  * \param the color to use to fill the polygon
  *
  * This algorithm is based on http://alienryderflex.com/polygon_fill/
  */
void fillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color);

/** \brief draws the outline of the given polygon
  * \param the vertices of a *closed* polygon (the first vertex must coincide with the first)
  * \param the color to use to draw the outline polygon
  */
void outlineDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color);

void outlineAndFillDrawer(SDL_Renderer* renderer, const std::vector<SDL_Point>& polygonPoints, const SDL_Color& color);

#endif // DRAWERS_H