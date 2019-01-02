#ifndef BONUSPIECE_H
#define BONUSPIECE_H
#include "Piece.h"

enum class Behavior {
    ADD_LIFE,
    SLOW_DOWN,
    DESTROY_ALL
};

/** \brief a bonus piece */
class BonusPiece : public Piece
{
    private:
        Behavior m_behavior;

    public:
        BonusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex);

        virtual ~BonusPiece();
};

#endif // BONUSPIECE_H
