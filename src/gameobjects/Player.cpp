#include "Player.h"
#include "DisplayManager.h"
#include "constants.h"
#include "Point3.h"
#include "LinearTransition.h"
#include "constants.h"

// "C" is the file containing the definition of a single cube
Player::Player(Renderer* renderer, const Mat4& rotationMatrix) : Piece{renderer, "C"}
{
    m_interactionCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_KEYDOWN, this, true);
    DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_FINGERDOWN, m_interactionCrumb.get());
    m_currentLane = NUMBER_OF_LANES/2; // current lane is the central lane

    // center the player cube in the central lane
    float xPosition = xForLane(m_currentLane);
    float yPosition = DisplayManager::screenHeight()/2;

    // Moves the player inwards along the ground
    Point3 moveBy = (rotationMatrix * Point3{0.0f, 0.0f, 1.0f}) * MOVE_BY;
    setPosition(Point3{xPosition, yPosition, 0.0f} + moveBy);
    setTransformationMatrix(rotationMatrix); // rotates the player
}

void Player::onEvent(SDL_Event e)
{
    int direction = 0;
    if (e.type == SDL_KEYDOWN) { // debugging on pc
        if (e.key.keysym.sym == SDLK_RIGHT)
            direction = 1;
        else if (e.key.keysym.sym == SDLK_LEFT)
            direction = -1;
        else // unknown key
            return;
    } else if(e.type == SDL_FINGERDOWN) {
        if (e.tfinger.x > 0.5f)
            direction = 1;
        else
            direction = -1;
    } else {return;} // unknown event

    // invert direction if needed
    direction = m_invertedCommands ? -direction : direction;

    /* Left - right controls */
    if (direction == 1 && m_currentLane < NUMBER_OF_LANES - 1)
        m_currentLane++;
    else if (direction == -1 && m_currentLane > 0)
        m_currentLane--;

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
                                                             45.0f,
                                                             nullptr,
                                                             "game");

}

void Player::setInvertedCommands(bool inverted)
{
    m_invertedCommands = inverted;
}

void Player::setInvincible()
{
    m_invincible = true;
    LinearTransition<float>::create(0.0f, 0.0f, nullptr, INVINCIVILITY_DURATION, [this](){this->m_invincible = false;}, "game");
}

bool Player::isInvincible()
{
    return m_invincible;
}

Player::~Player()
{

}
