#include "Player.h"
#include "DisplayManager.h"
#include "constants.h"
#include "Point3.h"
#include "LinearTransition.h"
#include "constants.h"
#include "EventManager.h"

// "C" is the file containing the definition of a single cube
Player::Player(Renderer* renderer, const Mat4& rotationMatrix) : Piece{renderer, "C"}, m_rotationMatrix{rotationMatrix}
{
    m_interactionCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_KEYDOWN, this, true);
    DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_FINGERUP, m_interactionCrumb.get());
    DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, m_interactionCrumb.get());
    m_currentLane = NUMBER_OF_LANES/2; // current lane is the central lane

    // center the player cube in the central lane
    float xPosition = xForLane(m_currentLane);
    float yPosition = DisplayManager::screenHeight()/2;

    // Moves the player inwards along the ground
    Point3 moveBy = (rotationMatrix * Point3{0.0f, 0.0f, 1.0f}) * MOVE_BY;
    m_initialPosition = Point3{xPosition, yPosition, 0.0f} + moveBy;
    setPosition(m_initialPosition);
    setTransformationMatrix(rotationMatrix); // rotates the player

    m_gravity = rotationMatrix * Point3{0.0f, 0.005f, 0.0f};
}

void Player::onEvent(SDL_Event e)
{
    if (e.type == EventManager::ENTER_FRAME_EVENT) {
        float delta = (*(static_cast<Uint32*>(e.user.data1)));

        // physics calculations: add gravity to the current velocity and set the position accordingly
        m_velocity += m_gravity * delta;
        Point3 pos = getPosition();
        pos += m_velocity * delta;
        setPosition(pos);

        // avoid falling into the ground
        if (getWorldPosition().y > m_initialPosition.y) {
            Point3 pos = getWorldPosition();
            pos.y = m_initialPosition.y;
            pos.z = m_initialPosition.z;
            setPosition(pos);
        }
    }

    int direction = 0;
    if (e.type == SDL_KEYDOWN) { // debugging on pc
        if (e.key.keysym.sym == SDLK_RIGHT)
            direction = 1;
        else if (e.key.keysym.sym == SDLK_LEFT)
            direction = -1;
        else if (e.key.keysym.sym == SDLK_UP && getWorldPosition().y >= m_initialPosition.y)
            m_velocity = m_rotationMatrix * Point3{0.0f, -1.1f, 0.0f};
        else // unknown key
            return;
    } else if(e.type == SDL_FINGERUP) {
        if (e.tfinger.x > 0.67f)
            direction = 1;
        else if (e.tfinger.x < 0.33f)
            direction = -1;
        else if (getWorldPosition().y >= m_initialPosition.y)
            m_velocity = m_rotationMatrix * Point3{0.0f, -1.1f, 0.0f};
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
