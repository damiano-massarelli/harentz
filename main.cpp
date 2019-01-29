#include <iostream>
#include <SDL.h>
#include <SDL_gpu.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"
#include "AudioManager.h"

#include "LocalLeaderboardScene.h"
#include "CreditsScene.h"

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);
    displayManager->setCurrentScene(new CreditsScene{});

    // pre loaded sounds
    AudioManager* audio = AudioManager::getInstance();
    audio->loadSound("resources/sound/glass1.wav");
    audio->loadSound("resources/sound/laser.wav");

    displayManager->startMainLoop();

    return 0;
}

