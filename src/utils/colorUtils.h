#ifndef COLORUTILS_H
#define COLORUTILS_H
#include <SDL.h>
#include <iostream>

SDL_Color operator*(const SDL_Color& color1, const SDL_Color& color2);

SDL_Color operator*(const SDL_Color& color, float f);

SDL_Color operator-(const SDL_Color& color1, const SDL_Color& color2);

SDL_Color operator+(const SDL_Color& color1, const SDL_Color& color2);

std::ostream& operator<<(std::ostream& out, const SDL_Color& color);

#endif // COLORUTILS_H
