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
            DESTROY_ALL
        };

        Behavior m_behavior;
        bool m_alreadyCollided = false; ///< BonusMalus pieces are not removed when they collide. An additional flag is hence needed.

    public:
        BonusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex) override;

        virtual ~BonusPiece();
};

#endif // BONUSPIECE_H
