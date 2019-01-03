#include "GameScene.h"
#include "Ground.h"
#include "constants.h"
#include "DisplayManager.h"
#include "BspRenderer.h"
#include "PaintersRenderer.h"

#include "Text.h"

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

    Text* t = new Text{screen, "resources/font/pixelUnicode"};
    t->setText("Hi there! I'm a text\nAnd I'm a new line. How exciting");
    add(t);
}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    //std::cout << "fps " << 1000/delta << std::endl;

    m_starFieldEffect->update(delta);

    // Moves all the pieces towards the player
    m_pieceManager->update(delta, this);
}

void GameScene::onRenderingComplete()
{
    m_effectRenderer->renderToScreen();
    m_3dRenderer->renderToScreen();
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

GameScene::~GameScene()
{
    //dtor
}
