#include "BackfaceCulling.h"

BackfaceCulling::BackfaceCulling(const Point3& playerPosition, const Point3& viewDirection) : m_playerPosition{playerPosition},
                                                                                              m_viewDirection{viewDirection}
{
    //ctor
}

bool BackfaceCulling::shouldRender(const Face& face)
{
    // Vector from the face center to the camera
    Point3 fromFaceToCamera = m_playerPosition - face.getCenter();
    return dot(fromFaceToCamera, face.getNormal()) > 0.0f;
}


BackfaceCulling::~BackfaceCulling()
{
    //dtor
}
