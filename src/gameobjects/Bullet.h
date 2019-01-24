#ifndef BULLET_H
#define BULLET_H
#include "Piece.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include <memory>

class Bullet : public Piece, public EventListener
{
    private:
        std::unique_ptr<EventListenerCrumb> m_enterFrameCrumb;

        float m_yRotation = 0.0f;

    public:
        Bullet(Renderer* renderer);

        virtual void handleCollision(int collidedCubeIndex) override {}; // nothing to do

        virtual void onEvent(SDL_Event e) override;

        virtual ~Bullet();
};

#endif // BULLET_H
