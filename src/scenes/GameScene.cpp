#include "GameScene.h"
#include<algorithm>

GameScene::GameScene()
{

}

void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
     Scene::onShow(window, renderer);
     m_3dRenderer = std::make_unique<Renderer>(renderer, 640, 480, 0, -500);

}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    elapsedFrames++;
    if (elapsedFrames % 300 == 0) {
        std::unique_ptr<Piece> piece = std::make_unique<Piece>(m_3dRenderer.get(), "L");
        add(piece.get());
        piece->setPosition(Point3{0.0f, 120.0f, 2500.0f});
        m_pieces.push_back(std::move(piece));

    }

    for (auto& piece : m_pieces) {
        Point3 curr = piece->getPosition();
        curr.z -= 2.0f;
        piece->setPosition(curr);
    }

    m_pieces.erase(std::remove_if(m_pieces.begin(), m_pieces.end(),
                                   [](const auto& p) {return p->getPosition().z <= -400.0f;}), m_pieces.end());
}


GameScene::~GameScene()
{
    //dtor
}
