#include "GameScene.h"
#include "Ground.h"
#include "constants.h"
#include "DisplayManager.h"
#include "BspRenderer.h"
#include "PaintersRenderer.h"
#include "LinearTransition.h"
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
    m_scoreText->setText("Score: 0");
    add(m_scoreText.get());

    onResume(EventStatus::DID);
}

void GameScene::onEnterFrame(SDL_Event& e)
{
    Scene::onEnterFrame(e);
    if (m_paused) return; // do not do anything while game is paused

    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    //std::cout << "fps " << 1000/delta << std::endl;

    m_starFieldEffect->update(delta);

    // Moves all the pieces towards the player
    m_pieceManager->update(delta, this);

    // Updates the
    m_scoreText->setText("Score: " + std::to_string(m_score));
}

void GameScene::onRenderingComplete()
{
    m_3dRenderer->renderToScreen();
    m_effectRenderer->renderToScreen();
}

void GameScene::onPause(const EventStatus& status)
{
    if (status == EventStatus::WILL)
        m_paused = true;
}

void GameScene::onResume(const EventStatus& status)
{
    if (status != EventStatus::DID) return;

    std::shared_ptr<Text> cntdwnText = std::make_shared<Text>(getScreen(), "resources/font/invasion2000");
    cntdwnText->setX(50);
    cntdwnText->setY(50);

    showOnResumeText(3);
}

void GameScene::startResumeCountdown(int countdown) const
{
    if (countdown < 0) return;
    LinearTransition<float>::create(0.0f, 1.0f, nullptr, 1000.0f, [this, countdown](){this->showOnResumeText(countdown-1);}, "game");
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

PieceManager* GameScene::getPieceManager()
{
    return m_pieceManager.get();
}

StarField* GameScene::getStarFieldEffect()
{
    return m_starFieldEffect.get();
}

GameScene::~GameScene()
{
    //dtor
}
