#include <iostream>
#include <SDL.h>
#include <SDL_gpu.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"

#include "AudioManager.h"

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);
    displayManager->setCurrentScene(new GameScene{});

    AudioManager::getInstance()->playMusic("resources/sound/base.mp3", -1);

    displayManager->startMainLoop();

    return 0;
}

