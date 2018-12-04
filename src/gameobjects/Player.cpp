#include "Player.h"
#include "DisplayManager.h"
#include "constants.h"
#include "Point3.h"
#include "LinearTransition.h"

// "C" is the file containing the definition of a single cube
Player::Player(Renderer* renderer, const Mat4& rotationMatrix) : Piece{renderer, "C"}
{
    m_keydownListenerCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_KEYDOWN, this, true);
    m_currentLane = NUMBER_OF_LANES/2; // current lane is the central lane

    // center the player cube in the central lane
    float xPosition = xForLane(m_currentLane);
    float yPosition = DisplayManager::screenHeight()/2;

    // Moves the player inwards along the ground
    Point3 moveBy = (rotationMatrix * Point3{0.0f, 0.0f, 1.0f}) * MOVE_BY;
    setPosition(Point3{xPosition, yPosition, 0.0f} + moveBy);
    setTransformationMatrix(rotationMatrix); // rotates the player
    setColor(SDL_Color{25, 190, 10});
}

void Player::onEvent(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN) { // Should always be KEYDOWN but you never know
        // Go up
        if (e.key.keysym.sym == SDLK_UP && !m_flying) {
            LinearTransition<float>::create(getPosition().y,
                                            getPosition().y - Piece::getCubeSide()*1.0f, // enough to jump a cube
                                            [this](float y) {
                                                Point3 current = this->getPosition();
                                                current.y = y;
                                                this->setPosition(current);
                                            },
                                            150.0f,
                                            nullptr,
                                            "game"); // end first transition

            m_flying = true;
        }
        // Go down
        else if (e.key.keysym.sym == SDLK_DOWN && m_flying) {
            LinearTransition<float>::create(getPosition().y,
                                            getPosition().y + Piece::getCubeSide()*1.0f, // enough to jump a cube
                                            [this](float y) {
                                                Point3 current = this->getPosition();
                                                current.y = y;
                                                this->setPosition(current);
                                            },
                                            150.0f,
                                            nullptr,
                                            "game"); // end first transition

            m_flying = false;
        }

        /* Left - right controls */
        if (e.key.keysym.sym == SDLK_RIGHT && m_currentLane < NUMBER_OF_LANES - 1)
            m_currentLane++;
        else if (e.key.keysym.sym == SDLK_LEFT && m_currentLane > 0)
            m_currentLane--;
        else // unknown key
            return;

        // delete the existing transition
        if (m_horizontalTransition)
            m_horizontalTransition->cancel();

        // moves the cube to the correct lane
        m_horizontalTransition = LinearTransition<float>::create(getPosition().x,
                                                                 xForLane(m_currentLane),
                                                                 [this](float x){
                                                                    Point3 current = this->getPosition();
                                                                    current.x = x;
                                                                    this->setPosition(current);
                                                                 },
                                                                 110.0f,
                                                                 nullptr,
                                                                 "game");
    }
}

float Player::xForLane(int lane) const
{
    return (lane * Piece::getCubeSide()) - DisplayManager::screenWidth()/2.0f + Piece::getCubeSide()/2;
}


Player::~Player()
{

}
