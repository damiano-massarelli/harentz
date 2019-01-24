#include "Bullet.h"
#include "Mat4.h"
#include "DisplayManager.h"

Bullet::Bullet(Renderer* renderer) : Piece{renderer, "C"}
{
    /* Starts listening for enterframe events */
    m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);

    auto scaleMatrix = scale(0.3f, 0.3f, 0.3f);
    getChildren()[0]->setTransformationMatrix(scaleMatrix);

    setFillColor(SDL_Color{0, 0, 255, 0});
}

void Bullet::onEvent(SDL_Event e)
{
    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    Mat4 tranformMatrix = rotation(0.0f, m_yRotation, 0.0f) * scale(0.3f, 0.3f, 0.3f);
    m_yRotation += 6.28f * delta/1000.0f; // One complete rotation per second
    if (getChildren().size() > 0)
        getChildren()[0]->setTransformationMatrix(tranformMatrix);
}


Bullet::~Bullet()
{
    //dtor
}
