#include "MalusPiece.h"
#include "DisplayManager.h"
#include "GameScene.h"
#include "SDL.h"
#include "LinearTransition.h"
#include "Player.h"
#include "randomUtils.h"
#include "AudioManager.h"

std::map<MalusPiece::Behavior, SDL_Color> MalusPiece::behavior2color{
    {Behavior::WIREFRAME_ONLY, SDL_Color{254, 242, 0, 255}},
    {Behavior::SPEED_UP, SDL_Color{255, 148, 0, 255}},
    {Behavior::INVERT_COMMANDS, SDL_Color{238, 28, 37, 255}}
};

MalusPiece::MalusPiece(Renderer* renderer) : BonusMalusPiece{renderer}
{
    m_behavior = static_cast<Behavior>(randRangeInt(0, static_cast<int>(Behavior::COUNT)));

    setFillColor(behavior2color[m_behavior]);
}

void MalusPiece::handleCollision(int collidedCubeIndex)
{
    BonusMalusPiece::handleCollision(collidedCubeIndex);
    AudioManager::getInstance()->playSound("resources/sound/malus.wav");

    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene());

    if (m_behavior == Behavior::WIREFRAME_ONLY) {
        gameScene->setMessage("Outline", BONUS_MALUS_TEXT_DURATION);
        for (auto& piece : gameScene->getPieceManager()->getPieces())
            piece->setFillColor(SDL_Color{0, 0, 0, 0});
        gameScene->getPieceManager()->setWireframeOnly(true);
        LinearTransition<int>::create(0, 0, nullptr, 5000.0f, [gameScene](){gameScene->getPieceManager()->setWireframeOnly(false);}, "game");
    }

    else if (m_behavior == Behavior::INVERT_COMMANDS) {
        Player* player = gameScene->getPlayer();
        player->setInvertedCommands(true);
        LinearTransition<int>::create(0, 0, nullptr, 2000.0f, [player, gameScene]() {
                                        player->setInvertedCommands(false);
                                        gameScene->setMessage("Normal", BONUS_MALUS_TEXT_DURATION/2);
                                      }, "game");
        gameScene->setMessage("Inverted", BONUS_MALUS_TEXT_DURATION);
    }

    else if (m_behavior == Behavior::SPEED_UP) {
        PieceManager* pieceManager = gameScene->getPieceManager();
        pieceManager->changeSpeed(1.2f);
        LinearTransition<float>::create(1.2f, 1.0f, [pieceManager](float f) {
                                        // back to normal
                                        pieceManager->changeSpeed(f);
                                      }, 3500.0f, nullptr, "game");
        gameScene->setMessage("Fast", BONUS_MALUS_TEXT_DURATION);
    }
}

MalusPiece::~MalusPiece()
{

}
