#include "GameScene.h"

GameScene::GameScene()
{

}

void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
     Scene::onShow(window, renderer);


     m_3dRenderer = std::make_unique<Renderer>(renderer, 640, 480, 0, -500);

     ciccio = new Piece(m_3dRenderer.get(), "S");
     add(ciccio);
}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);

    int mx, my;
    int w, h;
    getWindowSize(&w, &h);
    SDL_GetMouseState(&mx, &my);
    ciccio->setPosition(Point3{mx - w/2, my - h/2, 0.0f});
}


GameScene::~GameScene()
{
    //dtor
}
