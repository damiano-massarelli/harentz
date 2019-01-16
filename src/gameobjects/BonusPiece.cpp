#include "BonusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"
#include "LinearTransition.h"
#include "constants.h"

BonusPiece::BonusPiece(Renderer* renderer) : BonusMalusPiece{renderer}
{
    m_behavior = Behavior::DESTROY_ALL;

    setFillColor(SDL_Color{0, 255, 0, 255});
}

void BonusPiece::handleCollision(int collidedCubeIndex)
{
    if (m_alreadyCollided)
        return;
    m_alreadyCollided = true;

    BonusMalusPiece::handleCollision(collidedCubeIndex);

    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());

    /* Behavior ADD_LIFE */
    if (m_behavior == Behavior::ADD_LIFE) { // Adds a life to the player
        gameScene->incrementLives(1);

    /* Behavior DESTROY_ALL */
    } else if (m_behavior == Behavior::DESTROY_ALL) { // Destroys all the pieces currently on the screen
        auto& pieces = gameScene->getPieceManager()->getPieces();
        for (auto& piece : pieces) {
            int numOfCubes = static_cast<int>(piece->getChildren().size());

            /* Do not delete this piece and the other bonuses maluses which are composed of a single cube */
            if (piece.get() == this || numOfCubes == 1) continue;
            for (int cubeIndex = numOfCubes - 1; cubeIndex >= 0; --cubeIndex) {
                piece->explodeCube(cubeIndex);
                gameScene->incrementScore(SCORE_PER_PIECE);
            }
        }
    }

    /* Temporarily changes the color of the star effect */
    gameScene->getStarFieldEffect()->enableBonusFinalColor();
}

BonusPiece::~BonusPiece()
{
    //dtor
}
