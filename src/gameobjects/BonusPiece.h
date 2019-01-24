#ifndef BONUSPIECE_H
#define BONUSPIECE_H
#include "Renderer.h"
#include "BonusMalusPiece.h"
#include <map>
#include <SDL.h>

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
        static std::map<Behavior, SDL_Color> behavior2color; ///< maps a behavior to a color

        Behavior m_behavior;

    public:
        BonusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex) override;

        virtual ~BonusPiece();
};

#endif // BONUSPIECE_H
