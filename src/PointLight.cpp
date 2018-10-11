#include "PointLight.h"

PointLight::PointLight(const Point3& pos, const SDL_Color& color) : m_position{pos}, m_color{color}
{

}

Point3 PointLight::getPolygonCenter(const std::vector<std::reference_wrapper<Point3>>& polygonVertices)
{
    float totX{0};
    float totY{0};
    float totZ{0};
    int numElem = polygonVertices.size();
    for (const auto& wrappedPoint : polygonVertices) {
        const Point3& pt = wrappedPoint.get();
        totX += pt.x;
        totY += pt.y;
        totZ += pt.z;
    }
    return Point3{totX/numElem, totY/numElem, totZ/numElem};
}

float PointLight::getDimFactor(const std::vector<std::reference_wrapper<Point3>>& polygonVertices)
{
    Point3 fromPolygonToLight = normalized(m_position - getPolygonCenter(polygonVertices));
    Point3 polygonNormal = normalized(cross(polygonVertices[0].get() - polygonVertices[1].get(),
                                            polygonVertices[2].get() - polygonVertices[1].get()));
    std::cout << polygonNormal << " " << fromPolygonToLight << " " << getPolygonCenter(polygonVertices) << "\n";
    // negative values are not good
    return std::max(0.0f, dot(fromPolygonToLight, polygonNormal));
}


Point3& PointLight::getPosition()
{
    return m_position;
}

SDL_Color PointLight::getColorForPolygon(const std::vector<std::reference_wrapper<Point3>>& polygonVertices, const SDL_Color& polygonColor)
{
    float dimFactor = getDimFactor(polygonVertices);
    return SDL_Color{polygonColor.r * dimFactor, polygonColor.g * dimFactor, polygonColor.b * dimFactor, polygonColor.a};
}

PointLight::~PointLight()
{

}
