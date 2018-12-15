#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "Transform.h"
#include "Piece.h"
#include "Player.h"

/** \brief detects a collision between two object represented by spheres
  * \param transform1 a transform object
  * \param transform2 another transform object
  * \param radius the radius of the two spheres representing the objects
  * \return true if the objects are colliding, false otherwise */
bool sphereDetection(const Transform* transform1, const Transform* transform2, float radius);

/** \brief check whether a cube that is part of a piece is colliding with the player
  * \param piece a piece composed of a number of cubes
  * \param the player
  * \return the index of the cube that is colliding with the player, -1 if none. */
int collidingCube(const Piece* piece, const Player* player);

#endif // COLLISIONDETECTION_H
