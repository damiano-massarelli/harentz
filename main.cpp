#include <iostream>
#include <SDL.h>
#include <SDL_gpu.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"
#include "AudioManager.h"

int main(int argc, char* argv[])
{
    bool fitScreen = false;
    #ifdef __ANDROID__
        fitScreen = true;
    #endif // __ANDROID__
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT, fitScreen);
    displayManager->setCurrentScene(new GameScene{});

    // pre loaded sounds
    AudioManager* audio = AudioManager::getInstance();
    audio->loadSound("resources/sound/glass1.wav");
    audio->loadSound("resources/sound/laser.wav");

    displayManager->startMainLoop();

    return 0;
}

