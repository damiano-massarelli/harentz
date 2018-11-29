#include <iostream>
#include <SDL2/SDL.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"

#include "TransitionManager.h"

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);

    displayManager->setCurrentScene(new GameScene{});

    //LinearTransition a{150.0f};

    displayManager->startMainLoop();



    return 0;
}

