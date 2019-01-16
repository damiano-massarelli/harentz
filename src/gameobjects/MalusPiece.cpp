#include "MalusPiece.h"
#include "DisplayManager.h"
#include "GameScene.h"
#include "SDL.h"

MalusPiece::MalusPiece(Renderer* renderer) : BonusMalusPiece{renderer}
{
    m_behavior = Behavior::WIREFRAME_ONLY;
}

void MalusPiece::handleCollision(int collidedCubeIndex)
{
    BonusMalusPiece::handleCollision(collidedCubeIndex);

    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());

    if (m_behavior == Behavior::WIREFRAME_ONLY) {
        for (auto& piece : gameScene->getPieceManager()->getPieces())
            piece->setFillColor(SDL_Color{0, 0, 0, 0});
    }
}

MalusPiece::~MalusPiece()
{

}
