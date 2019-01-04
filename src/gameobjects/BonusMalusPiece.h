#ifndef BONUSMALUSPIECE_H
#define BONUSMALUSPIECE_H
#include "Piece.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include <memory>

/** \brief A class the contains all the common parts of bonus and malus pieces */
class BonusMalusPiece : public Piece, public EventListener
{
    private:
        std::unique_ptr<EventListenerCrumb> m_enterFrameCrumb;

        float m_yRotation = 0.0f;


    public:
        BonusMalusPiece(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex);

        virtual void onEvent(SDL_Event e);

        virtual ~BonusMalusPiece();
};

#endif // BONUSMALUSPIECE_H
