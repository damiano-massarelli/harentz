#include "GameScene.h"
#include<algorithm>

GameScene::GameScene()
{

}

void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
    Scene::onShow(window, renderer);
    m_3dRenderer = std::make_unique<Renderer>(renderer, 640, 480, 0, -500);
    m_3dRenderer->setLight(std::make_unique<PointLight>(Point3{0.0f, -120.0f, 0.0f}, SDL_Color{255, 255, 255, 255}, 0.4f));
    m_3dRenderer->setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, -500.0f} ,Point3{0.0f, 0.0f, 1.0f}));

}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    std::cout << 1000.0f/ (*(static_cast<Uint32*>(e.user.data1))) << "\n";

    elapsedFrames++;
    if (elapsedFrames == 1) {
        std::unique_ptr<Piece> piece = std::make_unique<Piece>(m_3dRenderer.get(), "T");
        add(piece.get());
        piece->setPosition(Point3{-40.0f, -500.0f, 500.0f});
        Mat4 rotation {{{1.0f, -0.0f, 0.0f, 0.0f},
                        {0.0f, 0.707f, -0.707f, 0.0f},
                        {0.0f, 0.707f, 0.707f, 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}} };

        piece->setTransformationMatrix(rotation);

        m_pieces.push_back(std::move(piece));

    }

    for (auto& piece : m_pieces) {
        Point3 curr = piece->getPosition();
        curr.z -= 2.0f;
        curr.y += 2.0f;
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
