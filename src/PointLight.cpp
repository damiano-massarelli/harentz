#include "PointLight.h"

PointLight::PointLight(const Point3& pos, const SDL_Color& color, float ambientFactor) : m_position{pos},
                        m_color{color}, m_ambientFactor{ambientFactor}
{

}

float PointLight::getDimFactor(const Face& face)
{
    Point3 fromfaceToLight = normalized(m_position - face.getCenter());
    Point3 faceNormal = normalized(face.getNormal());
    // max: negative values are not good, min: with the ambient factor the result might be greater than 1.0f
    return std::min(std::max(0.0f, dot(fromfaceToLight, faceNormal)) + m_ambientFactor, 1.0f);
}


Point3& PointLight::getPosition()
{
    return m_position;
}

SDL_Color PointLight::getColorForFace(const Face& face, const SDL_Color& faceColor)
{
    float dimFactor = getDimFactor(face);
    return SDL_Color{faceColor.r * dimFactor, faceColor.g * dimFactor, faceColor.b * dimFactor, faceColor.a};
}

PointLight::~PointLight()
{

}
