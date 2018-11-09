#include "GameScene.h"
//#include "Piece.h"

// TO REMOVE
#include "AbstractRenderable.h"
#include "Point3.h"
#include "Mat3.h"
#include "Shape.h"
#include "Transform.h"


Shape sCube(std::vector<Point3>{ Point3{-20.0f, -20.0f, -20.0f}, Point3{20.0f, -20.0f, -20.0f},
                             Point3{20.0f, 20.0f, -20.0f}, Point3{-20.0f, 20.0f, -20.0f},
                             Point3{20.0f, -20.0f, 40.0f}, Point3{20.0f, 20.0f, 40.0f},
                             Point3{-20.0f, -20.0f, 40.0f}, Point3{-20.0f, 20.0f, 40.0f}},
        std::vector<int>{1, 4, 5, 2, // right face
                        0, 3, 7, 6, // left face
                        0, 1, 2, 3, // front face
                        0, 6, 4, 1}); // top face


GameScene::GameScene()
{

}

void GameScene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
     Scene::onShow(window, renderer);


     m_3dRenderer = std::make_unique<Renderer>(renderer, 640, 480, 0, -500);


     add(new Transform{sCube});
}

void GameScene::onEvent(SDL_Event e)
{
    Scene::onEvent(e);
}


GameScene::~GameScene()
{
    //dtor
}
