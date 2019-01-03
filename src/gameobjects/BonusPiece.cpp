#include "BonusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"

BonusPiece::BonusPiece(Renderer* renderer) : Piece{renderer, "C"}
{
    m_behavior = Behavior::ADD_LIFE;

    /* Starts listening for enterframe events */
    m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);

    /* Reduce the size of the piece */
    Mat4 scaleMatrix = scale(0.5, 0.5, 0.5);
    getChildren()[0]->setTransformationMatrix(scaleMatrix);

    setFillColor(SDL_Color{0, 255, 0, 128});
}

void BonusPiece::handleCollision(int collidedCubeIndex)
{
    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());
    Player* player = gameScene->getPlayer();
    player->setLives(player->getLives() + 1);

    removeCube(collidedCubeIndex);
}

void BonusPiece::onEvent(SDL_Event e)
{
    Mat4 tranformMatrix = rotation(0.0f, m_yRotation, 0.0f) * scale(0.5f, 0.5f, 0.5f);
    m_yRotation += 0.09f;
    if (getChildren().size() > 0)
        getChildren()[0]->setTransformationMatrix(tranformMatrix);
}


BonusPiece::~BonusPiece()
{
    //dtor
}
