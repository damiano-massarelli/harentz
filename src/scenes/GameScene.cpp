#include "GameScene.h"
#include<algorithm>
#include "Ground.h"
#include "drawers.h"
#include "constants.h"
#include "DisplayManager.h"

GameScene::GameScene()
{

}

void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
    Scene::onShow(window, renderer);

    int screenWidth = DisplayManager::screenWidth();
    int screenHeight = DisplayManager::screenHeight();

    m_3dRenderer = std::make_unique<Renderer>(renderer, screenWidth, screenHeight, SCREEN_Z, PROJECTION_POINT_Z);
    m_3dRenderer->setLight(std::make_unique<PointLight>(Point3{0.0f, -120.0f, 0.0f}, SDL_Color{255, 255, 255, 255}, 0.25f));
    m_3dRenderer->setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, PROJECTION_POINT_Z}, Point3{0.0f, 0.0f, 1.0f}));

    // Creates the renderer for the ground
    m_groundRenderer = std::make_unique<Renderer>(renderer, screenWidth, screenHeight, SCREEN_Z, PROJECTION_POINT_Z);
    m_groundRenderer->setDrawer(outlineDrawer);

    // creates and adds the ground to the scene
    m_ground = std::make_unique<Ground>(m_groundRenderer.get(), screenWidth, screenHeight, NUMBER_OF_LANES);
    add(m_ground.get());

    // rotation and spawn point are dictated by the rotation of the ground
    m_rotationMatrix = m_ground->getRotationMatrix();
    m_spawnPoint = m_ground->getSpawnPoint();

    // Creates the player
    m_player = std::make_unique<Player>(m_3dRenderer.get(), m_rotationMatrix);
    add(m_player.get());
}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    std::cout << 1000.0f/ (*(static_cast<Uint32*>(e.user.data1))) << "\n";

    elapsedFrames++;
    if (elapsedFrames == 1) {
        std::unique_ptr<Piece> piece = std::make_unique<Piece>(m_3dRenderer.get(), "S");
        add(piece.get());
        piece->setPosition(Point3{0.0f, m_spawnPoint.y, m_spawnPoint.z});

        piece->setTransformationMatrix(m_rotationMatrix);

        m_pieces.push_back(std::move(piece));

    }

    for (auto& piece : m_pieces) {
        Point3 curr = piece->getPosition();
        curr = curr + (m_rotationMatrix * Point3{0.0f, 0.0f, -1.0f} * 5);
        piece->setPosition(curr);
    }

    m_pieces.erase(std::remove_if(m_pieces.begin(), m_pieces.end(),
                                   [](const auto& p) {return p->getPosition().z <= -200.0f;}), m_pieces.end());

}

void GameScene::onRenderingComplete()
{
    m_groundRenderer->renderToScreen();
    m_3dRenderer->renderToScreen();
}

void GameScene::onRemove()
{
    // deletes all the transitions generated. This is necessary as these transitions operate
    // on objects that will be destroyed
    DisplayManager::getInstance()->getTransitionManager().cancelByTag("game");
}


GameScene::~GameScene()
{
    //dtor
}
