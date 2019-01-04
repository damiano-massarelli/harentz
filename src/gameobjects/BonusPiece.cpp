#include "BonusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"
#include "LinearTransition.h"

BonusPiece::BonusPiece(Renderer* renderer) : BonusMalusPiece{renderer}
{
    m_behavior = Behavior::ADD_LIFE;

    setFillColor(SDL_Color{0, 255, 0, 255});
}

void BonusPiece::handleCollision(int collidedCubeIndex)
{
    BonusMalusPiece::handleCollision(collidedCubeIndex);

    // Adds a life to the player
    if (m_behavior == Behavior::ADD_LIFE) {
        GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());
        Player* player = gameScene->getPlayer();
        player->setLives(player->getLives() + 1);
    }
}

BonusPiece::~BonusPiece()
{
    //dtor
}
