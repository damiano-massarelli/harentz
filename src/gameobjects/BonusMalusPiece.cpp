#include "BonusMalusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"
#include "LinearTransition.h"

BonusMalusPiece::BonusMalusPiece(Renderer* renderer) : Piece{renderer, "C"}
{
    /* Starts listening for enterframe events */
    m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);

    /* Reduce the size of the piece */
    Mat4 scaleMatrix = scale(0.5, 0.5, 0.5);
    getChildren()[0]->setTransformationMatrix(scaleMatrix);
}

void BonusMalusPiece::handleCollision(int collidedCubeIndex)
{
    removeCube(0);
}

void BonusMalusPiece::onEvent(SDL_Event e)
{
    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    Mat4 tranformMatrix = rotation(0.0f, m_yRotation, 0.0f) * scale(0.5f, 0.5f, 0.5f);
    m_yRotation += 6.28f * delta/1000.0f; // One complete rotation per second
    if (getChildren().size() > 0)
        getChildren()[0]->setTransformationMatrix(tranformMatrix);
}


BonusMalusPiece::~BonusMalusPiece()
{

}
