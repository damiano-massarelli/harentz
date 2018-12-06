#ifndef COLORUTILS_H
#define COLORUTILS_H
#include <SDL2/SDL.h>

SDL_Color operator*(const SDL_Color& color1, const SDL_Color& color2);

SDL_Color operator*(const SDL_Color& color, float f);

#endif // COLORUTILS_H
