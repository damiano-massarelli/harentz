#ifndef BONUSPIECE_H
#define BONUSPIECE_H
#include "Renderer.h"
#include "BonusMalusPiece.h"

/** \brief a bonus piece */
class BonusPiece : public BonusMalusPiece
{
    private:
        enum class Behavior {
            ADD_LIFE,
            SLOW_DOWN,
            DESTROY_ALL,
            COUNT
        };

        Behavior m_behavior;

    public:
        BonusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex) override;

        virtual ~BonusPiece();
};

#endif // BONUSPIECE_H
