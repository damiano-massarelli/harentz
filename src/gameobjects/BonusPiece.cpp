#include "BonusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"
#include "LinearTransition.h"
#include "constants.h"
#include "randomUtils.h"
#include "constants.h"
#include "AudioManager.h"

// static members
std::map<BonusPiece::Behavior, SDL_Color> BonusPiece::behavior2color{
    {Behavior::ADD_LIFE, SDL_Color{0, 245, 5, 255}},
    {Behavior::SLOW_DOWN, SDL_Color{0, 166, 82, 255}},
    {Behavior::DESTROY_ALL, SDL_Color{1, 168, 158, 255}},
};

BonusPiece::BonusPiece(Renderer* renderer) : BonusMalusPiece{renderer}
{
    m_behavior = static_cast<Behavior>(randRangeInt(0, static_cast<int>(Behavior::COUNT)));

    setFillColor(behavior2color[m_behavior]);
}

void BonusPiece::handleCollision(int collidedCubeIndex)
{
    BonusMalusPiece::handleCollision(collidedCubeIndex);
    AudioManager::getInstance()->playSound("resources/sound/bonus.wav");

    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());

    /* Behavior ADD_LIFE */
    if (m_behavior == Behavior::ADD_LIFE) { // Adds a life to the player
        gameScene->incrementLives(1);
        gameScene->setMessage("1 UP", BONUS_MALUS_TEXT_DURATION);

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

    else if (m_behavior == Behavior::SLOW_DOWN) {
        PieceManager* pieceManager = gameScene->getPieceManager();
        pieceManager->changeSpeed(0.5f);
        LinearTransition<float>::create(0.5f, 1.0f, [pieceManager](float f) {
                                        // back to normal
                                        pieceManager->changeSpeed(f);
                                      }, 3500.0f, nullptr, "game");
        gameScene->setMessage("Slow", BONUS_MALUS_TEXT_DURATION);
    }

    /* Temporarily changes the color of the star effect */
    gameScene->getStarFieldEffect()->enableBonusFinalColor();
}

BonusPiece::~BonusPiece()
{
    //dtor
}
