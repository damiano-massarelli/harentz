#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "Transform.h"

/** \brief detects a collision between two object represented by spheres
  * \param transform1 a transform object
  * \param transform2 another transform object
  * \param radius the radius of the two spheres representing the objects
  * \return true if the objects are colliding, false otherwise */
bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius);

#endif // COLLISIONDETECTION_H
