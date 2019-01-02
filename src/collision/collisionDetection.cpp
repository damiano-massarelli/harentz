#include "collisionDetection.h"

bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius)
{
    return distance(transform1->getWorldPosition(), transform2->getWorldPosition()) < 2*radius;
}
