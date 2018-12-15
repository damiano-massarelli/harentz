#include "collisionDetection.h"

bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius)
{
    return distance(transform1->getWorldPosition(), transform2->getWorldPosition()) < 2*radius;
}

int collidingCube(const Piece* piece, const Player* player)
{
    const std::vector<Transform*>& children = piece->getChildren();
    for (int i = 0; i < children.size(); i++) {
        if (sphereDetection(children[i], player, Piece::getCubeSide()/2)) // Collision found return the index
            return i;
    }

    // No collision, return -1
    return -1;
}
