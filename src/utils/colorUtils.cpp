#include "colorUtils.h"

SDL_Color operator*(const SDL_Color& color1, const SDL_Color& color2) {
    float r = color1.r/255.0f;
    float g = color1.g/255.0f;
    float b = color1.b/255.0f;
    float a = color1.a/255.0f;
    return SDL_Color{static_cast<Uint8>(r * color2.r), static_cast<Uint8>(g * color2.g),
                     static_cast<Uint8>(b * color2.b), static_cast<Uint8>(a * color2.a)};
}

SDL_Color operator*(const SDL_Color& color, float f)
{
    return SDL_Color{static_cast<Uint8>(f * color.r), static_cast<Uint8>(f * color.g),
                     static_cast<Uint8>(f * color.b), color.a};
}

SDL_Color operator-(const SDL_Color& color1, const SDL_Color& color2)
{
    return SDL_Color{static_cast<Uint8>(color1.r - color2.r), static_cast<Uint8>(color1.g - color2.g),
                            static_cast<Uint8>(color1.b - color2.b), static_cast<Uint8>(color1.a - color2.a)};
}

SDL_Color operator+(const SDL_Color& color1, const SDL_Color& color2)
{
    return SDL_Color{static_cast<Uint8>(color1.r + color2.r), static_cast<Uint8>(color1.g + color2.g),
                            static_cast<Uint8>(color1.b + color2.b), static_cast<Uint8>(color1.a + color2.a)};
}


std::ostream& operator<<(std::ostream& out, const SDL_Color& color)
{
    out << "SDL_Color(" << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", " << static_cast<int>(color.b) << ", " << static_cast<int>(color.a) << ")";
    return out;
}

