#include <iostream>
#include <SDL2/SDL.h>
#include "DisplayManager.h"
#include "GameScene.h"
#include "constants.h"

#include "LinearTransition.h"

int main(int argc, char* argv[])
{

    DisplayManager* displayManager = DisplayManager::create("Harentz", INITIAL_WIDTH, INITIAL_HEIGHT);
    createLT<Point3>(Point3{3.0f}, Point3{10.0f}, [](Point3 a){std::cout << a << "\n";}, 500.0f);
    displayManager->setCurrentScene(new GameScene{});


    displayManager->startMainLoop();



    return 0;
}

