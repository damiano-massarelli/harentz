#include "PointLight.h"

PointLight::PointLight(const Point3& pos, const SDL_Color& color, float ambientFactor) : m_position{pos},
                        m_color{color}, m_ambientFactor{ambientFactor}
{

}

float PointLight::getDimFactor(const Polygon& polygon)
{
    Point3 fromPolygonToLight = normalized(m_position - polygon.getCenter());
    Point3 polygonNormal = normalized(polygon.getNormal());
    // max: negative values are not good, min: with the ambient factor the result might be greater than 1.0f
    return std::min(std::max(0.0f, dot(fromPolygonToLight, polygonNormal)) + m_ambientFactor, 1.0f);
}


Point3& PointLight::getPosition()
{
    return m_position;
}

SDL_Color PointLight::getColorForPolygon(const Polygon& polygon, const SDL_Color& polygonColor)
{
    float dimFactor = getDimFactor(polygon);
    return SDL_Color{polygonColor.r * dimFactor, polygonColor.g * dimFactor, polygonColor.b * dimFactor, polygonColor.a};
}

PointLight::~PointLight()
{

}
