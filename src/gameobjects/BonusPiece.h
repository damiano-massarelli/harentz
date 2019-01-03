#ifndef BONUSPIECE_H
#define BONUSPIECE_H
#include "Piece.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include <memory>

enum class Behavior {
    ADD_LIFE,
    SLOW_DOWN,
    DESTROY_ALL
};

/** \brief a bonus piece */
class BonusPiece : public Piece, public EventListener
{
    private:
        Behavior m_behavior;
        std::unique_ptr<EventListenerCrumb> m_enterFrameCrumb;

        float m_yRotation = 0.0f;

    public:
        BonusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex);

        virtual void onEvent(SDL_Event e);

        virtual ~BonusPiece();
};

#endif // BONUSPIECE_H
