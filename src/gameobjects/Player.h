#ifndef PLAYER_H
#define PLAYER_H
#include "Piece.h"
#include "Renderer.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include "LinearTransition.h"
#include "Mat4.h"
#include <memory>

class Player : public Piece, public EventListener
{
    private:
        static const int MOVE_BY = 150.f; ///< defines how much the player is moved inwards (distance from bottom of screen)
        int m_currentLane;  ///< the lane of the player
        bool m_flying = false; ///< is the player flying?
        Point3 m_upDirection; ///< the direction to follow when the cube is moved upwards

        std::unique_ptr<EventListenerCrumb> m_keydownListenerCrumb; ///< crumb for key events
        std::shared_ptr<LinearTransition<float>> m_horizontalTransition;


        /** \brief returns the x coordinate for lane */
        float xForLane(int lane) const;

    public:
        Player(Renderer* renderer, const Mat4& rotationMatrix);

        void onEvent(SDL_Event e) override;

        virtual ~Player();
};

#endif // PLAYER_H