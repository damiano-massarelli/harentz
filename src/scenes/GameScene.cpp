#include "GameScene.h"
#include "Ground.h"
#include "constants.h"
#include "DisplayManager.h"
#include "BspRenderer.h"
#include "PaintersRenderer.h"
#include "LinearTransition.h"
#include "LocalLeaderboardScene.h"
#include "AudioManager.h"
#include <sstream>

GameScene::GameScene()
{

}

void GameScene::onShow(GPU_Target* screen)
{
    Scene::onShow(screen);

    int screenWidth = DisplayManager::screenWidth();
    int screenHeight = DisplayManager::screenHeight();

    // Renderer for pieces
    m_3dRenderer = std::make_unique<BspRenderer>(screen, screenWidth, screenHeight, SCREEN_Z, PROJECTION_POINT_Z);
    m_3dRenderer->setLight(std::make_unique<PointLight>(Point3{0.0f, -120.0f, 0.0f}, SDL_Color{255, 255, 255, 255}, 0.15f));
    m_3dRenderer->setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, PROJECTION_POINT_Z}, Point3{0.0f, 0.0f, 1.0f}));

    // Creates the renderer for the ground
    m_effectRenderer = std::make_unique<PaintersRenderer>(screen, screenWidth, screenHeight, SCREEN_Z, PROJECTION_POINT_Z);
    m_3dRenderer->setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, PROJECTION_POINT_Z}, Point3{0.0f, 0.0f, 1.0f}));

    // creates and adds the ground to the scene
    m_ground = std::make_unique<Ground>(m_3dRenderer.get(), screenWidth, screenHeight, NUMBER_OF_LANES);
    add(m_ground.get());

    // rotation and spawn point are dictated by the rotation of the ground
    m_rotationMatrix = m_ground->getRotationMatrix();
    m_spawnPoint = m_ground->getSpawnPoint();

    // Creates the player
    m_player = std::make_unique<Player>(m_3dRenderer.get(), m_rotationMatrix);
    add(m_player.get());

    // Piece manager
    m_pieceManager = std::make_unique<PieceManager>(m_3dRenderer.get(), m_spawnPoint, m_rotationMatrix);

    m_starFieldEffect = std::make_unique<StarField>(screen);
    add(m_starFieldEffect.get());

    m_scoreText = std::make_unique<Text>(screen, "resources/font/invasion2000");
    m_scoreText->setText("score: 0");
    m_scoreText->setX(5);
    add(m_scoreText.get());

    m_livesText = std::make_unique<Text>(screen, "resources/font/invasion2000");
    m_livesText->setText("lives: " + std::to_string(m_lives));
    m_livesText->setX(5);
    m_livesText->setY(m_scoreText->getY() + m_scoreText->getHeight() + 5);
    add(m_livesText.get());

    /* creates the text to use for the countdown (shared ptr so that it can be passed to a lambda without issues)*/
    m_messageText = std::make_unique<Text>(screen, "resources/font/invasion2000_50");
    m_messageText->setY(100);
    add(m_messageText.get());

    onResume(EventStatus::DID);
}

void GameScene::onEnterFrame(SDL_Event& e)
{
    Scene::onEnterFrame(e);
    if (m_paused || m_gameOver) return; // do not do anything while game is paused

    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    //std::cout << "fps " << 1000/delta << std::endl;

    m_starFieldEffect->update(delta);

    // Updates the score
    m_scoreText->setText("score: " + std::to_string(m_score));

    // Moves all the pieces towards the player
    m_pieceManager->update(delta, this);
}

void GameScene::onRenderingComplete()
{
    m_3dRenderer->renderToScreen();
    m_effectRenderer->renderToScreen();
}

void GameScene::onPause(const EventStatus& status)
{
    if (status == EventStatus::WILL) {
        m_paused = true; // stops the gameplay until the app is resumed
        m_starFieldEffect->update(0.0f); // stops the starfield effect
    }
}

void GameScene::onResume(const EventStatus& status)
{
    // do not resume if game is over
    if (status != EventStatus::DID || m_gameOver) return;

    startResumeCountdown(3);
}

void GameScene::startResumeCountdown(int countdown)
{
    if (countdown < 0) {
        m_paused = false;
        setMessage("");
        return;
    };
    if (countdown == 0) {
        setMessage("Go!");
        AudioManager::getInstance()->playSound("resources/sound/beepHigh.wav");\
        AudioManager::getInstance()->playMusic("resources/sound/base.mp3", -1);
    }else {
        setMessage(std::to_string(countdown));
        AudioManager::getInstance()->playSound("resources/sound/beepLow.wav");
    }

    // the duration of this transition is 1s if the countdown is different from 0, 0.45 if is 0
    LinearTransition<float>::create(0.0f, 1.0f, nullptr, (countdown == 0 ? 450.0f : 1000.0f),
                                    [this, countdown](){
                                        this->startResumeCountdown(countdown-1);
                                    },
                                    "game");
}

void GameScene::onRemove()
{
    // deletes all the transitions generated. This is necessary as these transitions operate
    // on objects that will be destroyed
    DisplayManager::getInstance()->getTransitionManager().cancelByTag("game");
}

Renderer* GameScene::getEffectRenderer()
{
    return m_effectRenderer.get();
}

Player* GameScene::getPlayer()
{
    return m_player.get();
}

void GameScene::incrementScore(int inc)
{
    m_score += inc;
}

int GameScene::getScore() const
{
    return m_score;
}

void GameScene::incrementLives(int lives)
{
    if (lives < 0 && m_player->isInvincible()) // if the player is invincible it cannot be hit
        return;

    m_player->setInvincible(); // now the player cannot be hit for some time
    m_lives += lives;
    if (m_lives < 0) {
        m_gameOver = true;
        m_player->explodeCube(0);
        // wait till the end of the explosion to change scene
        LinearTransition<int>::create(0, 0, nullptr, 750.0f, [this](){
                                        DisplayManager::getInstance()->setCurrentScene(new LocalLeaderboardScene{this->m_score});
                                      }, "game");

        return;
    }
    SDL_Color color = m_player->getFillColor();
    if (m_lives >= 3)
        color.a = 255;
    if (m_lives == 2)
        color.a = 200;
    if (m_lives <= 1)
        color.a = 128;

    m_player->setFillColor(color);
    m_livesText->setText("lives: " + std::to_string(m_lives));

    if (lives < 0) { // play glass sound when losing lives
        AudioManager::getInstance()->playSound("resources/sound/glass1.wav");
    }
}

int GameScene::getLives() const
{
    return m_lives;
}

PieceManager* GameScene::getPieceManager()
{
    return m_pieceManager.get();
}

StarField* GameScene::getStarFieldEffect()
{
    return m_starFieldEffect.get();
}

void GameScene::setMessage(const std::string& message, float duration)
{
    m_messageText->setText(message);
    m_messageText->setX(DisplayManager::screenWidth()/2 - m_messageText->getWidth()/2);

    if (duration >= 0.0f)
        LinearTransition<float>::create(0.0f, 0.0f, nullptr, duration, [this](){this->m_messageText->setText("");}, "game");
}

GameScene::~GameScene()
{

}
