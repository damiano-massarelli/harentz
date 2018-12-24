#include "collisionDetection.h"

#include "DisplayManager.h"

bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius)
{
    return distance(transform1->getWorldPosition(), transform2->getWorldPosition()) < 2*radius;
}

int collidingCube(const Piece* piece, const Player* player)
{
    const std::vector<Transform*>& children = piece->getChildren();
    /** There is a difference between the position of a Piece and that of a cube inside
      * of it. Hence, we use the only cube the player has to detect collisions */
    const Transform* playerCube = player->getChildren()[0];

    for (std::size_t i = 0; i < children.size(); i++) {
        if (sphereDetection(children[i], playerCube, (Piece::getCubeSide()/2) * 0.9f)) { // Collision found return the index
            return i;
        }
    }

    // No collision, return -1
    return -1;
}
