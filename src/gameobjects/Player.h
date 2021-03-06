#ifndef PLAYER_H
#define PLAYER_H
#include "Piece.h"
#include "Renderer.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include "LinearTransition.h"
#include "Mat4.h"
#include "Point3.h"
#include <memory>

class Player : public Piece, public EventListener
{
    private:
        static const int MOVE_BY = 150.f; ///< defines how much the player is moved inwards (distance from bottom of screen)
        int m_currentLane;  ///< the lane of the player
        int m_lives = 3;
        bool m_invertedCommands = false;
        bool m_invincible = false;

        Point3 m_gravity;
        Point3 m_velocity;
        Point3 m_initialPosition;
        Mat4 m_rotationMatrix;
        std::unique_ptr<EventListenerCrumb> m_interactionCrumb; ///< crumb for interaction with user
        std::shared_ptr<LinearTransition<float>> m_horizontalTransition;

    public:
        Player(Renderer* renderer, const Mat4& rotationMatrix);

        void onEvent(SDL_Event e) override;

        void setInvertedCommands(bool inverted);

        void setInvincible();

        bool isInvincible();

        virtual ~Player();
};

#endif // PLAYER_H
