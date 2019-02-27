#include "collisionDetection.h"

bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius)
{
    if (transform1 == nullptr || transform2 == nullptr)
        return false;
    return distance(transform1->getWorldPosition(), transform2->getWorldPosition()) < 2*radius;
}
