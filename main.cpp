#include <iostream>
#include <SDL.h>
#include <SDL_gpu.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"

#include "LocalLeaderboardScene.h"
#include "ioUtils.h"

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);
    displayManager->setCurrentScene(new LocalLeaderboardScene{500});

    displayManager->startMainLoop();

    return 0;
}

