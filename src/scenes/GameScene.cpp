#include "GameScene.h"
#include<algorithm>
#include <Ground.h>
#include <cmath>

const float GameScene::SCREEN_Z = 0.0f;
const float GameScene::PROJECTION_POINT_Z = -500.0f;

GameScene::GameScene()
{

}

void GameScene::computeRotationMatrix()
{
    float angle = asin(m_spawnPoint.y / Ground::GROUND_DEPTH);
    m_rotationMatrix = rotation(-angle, 0.0f, 0.0f);
}


void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
    Scene::onShow(window, renderer);

    int screenWidth = -1;
    int screenHeight = -1;
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    std::cout << screenWidth << " " << screenHeight << "\n";


    m_3dRenderer = std::make_unique<Renderer>(renderer, screenWidth, screenHeight, SCREEN_Z, PROJECTION_POINT_Z);
    m_3dRenderer->setLight(std::make_unique<PointLight>(Point3{0.0f, -120.0f, 0.0f}, SDL_Color{255, 255, 255, 255}, 0.4f));
    m_3dRenderer->setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, PROJECTION_POINT_Z}, Point3{0.0f, 0.0f, 1.0f}));

    // Gets the point in which the new pieces should be spawned
    // This position is calculated so that pieces whose z coordinate is GROUND_DEPTH
    // Will be displayed on top of the screen
    m_spawnPoint = m_3dRenderer->inverseProjection(SDL_Point{0, -screenHeight/4}, Ground::GROUND_DEPTH);
    std::cout << m_spawnPoint << "\n";
    computeRotationMatrix();

    // creates and adds the ground to the scene
    m_ground = std::make_unique<Ground>(m_3dRenderer.get(), screenWidth, screenHeight);
    add(m_ground.get());
    std::cout << m_ground->getVertWorldPositions()[10] << "\n";
    m_ground->setTransformationMatrix(m_rotationMatrix);
    m_ground->resize();
    std::cout << m_ground->getVertWorldPositions()[10] << "\n";
}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    //std::cout << 1000.0f/ (*(static_cast<Uint32*>(e.user.data1))) << "\n";

    elapsedFrames++;
    if (elapsedFrames == 1) {
        std::unique_ptr<Piece> piece = std::make_unique<Piece>(m_3dRenderer.get(), "S");
        add(piece.get());
        piece->setPosition(Point3{0.0f, m_spawnPoint.y, Ground::GROUND_DEPTH});

        piece->setTransformationMatrix(m_rotationMatrix);

        m_pieces.push_back(std::move(piece));

    }

    for (auto& piece : m_pieces) {
        Point3 curr = piece->getPosition();
        //curr = curr + Point3{0.0f, 5.0f, 0.0f};//(Point3{0.0f, (1.0f - 0.94f), -(1.0f - 0.34f)} * 8.0f);
        piece->setPosition(curr);
    }

    m_pieces.erase(std::remove_if(m_pieces.begin(), m_pieces.end(),
                                   [](const auto& p) {return p->getPosition().z <= -400.0f;}), m_pieces.end());

}

void GameScene::onRenderingComplete()
{
    m_3dRenderer->renderToScreen();
}

GameScene::~GameScene()
{
    //dtor
}
