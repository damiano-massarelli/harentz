#include <iostream>
#include <SDL2/SDL.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"

constexpr int SCREEN_WIDTH = 410;
constexpr int SCREEN_HEIGHT = 700;

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);

    displayManager->setCurrentScene(new GameScene{});
    displayManager->startMainLoop();

    return 0;
}

