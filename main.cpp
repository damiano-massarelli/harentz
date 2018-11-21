#include <iostream>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include "Point3.h"
#include "PointLight.h"
#include <memory>
#include "BackfaceCulling.h"
#include "DisplayManager.h"
#include "Mat3.h"
#include "GameScene.h"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SCREEN_Z = 0;
constexpr int PROJECTION_POINT_Z = -500;

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", SCREEN_WIDTH, SCREEN_HEIGHT);

    displayManager->setCurrentScene(new GameScene{});
    displayManager->startMainLoop();

    return 0;
}

