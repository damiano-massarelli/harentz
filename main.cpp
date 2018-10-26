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

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SCREEN_Z = 0;
constexpr int PROJECTION_POINT_Z = -500;

int main(int argc, char* argv[])
{
    DisplayManager* displayManager = DisplayManager::create("Harentz", SCREEN_WIDTH, SCREEN_HEIGHT);

    Renderer renderer(displayManager->getRenderer(), SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_Z, PROJECTION_POINT_Z);

    renderer.setLight(std::make_unique<PointLight>(Point3{0.0f, -100.0f, -200.0f}, SDL_Color{255, 0, 0, 255}, 0.2));

    renderer.setBackfaceCulling(std::make_unique<BackfaceCulling>(Point3{0.0f, 0.0f, PROJECTION_POINT_Z} ,Point3{0.0f, 0.0f, 1.0f}));

    Shape sCube(std::vector<Point3>{ Point3{-20.0f, -20.0f, -20.0f}, Point3{20.0f, -20.0f, -20.0f},
                                     Point3{20.0f, 20.0f, -20.0f}, Point3{-20.0f, 20.0f, -20.0f},
                                     Point3{20.0f, -20.0f, 40.0f}, Point3{20.0f, 20.0f, 40.0f},
                                     Point3{-20.0f, -20.0f, 40.0f}, Point3{-20.0f, 20.0f, 40.0f}},
                std::vector<int>{1, 4, 5, 2, // right face
                                0, 3, 7, 6, // left face
                                0, 1, 2, 3, // front face
                                0, 6, 4, 1}); // top face

    Transform tCube(sCube, Point3{-100.0f, 100.0f, 900.0f});
    tCube.setTransformationMatrix(Mat3{Point3{1.0f, 0.0f, 0.0f}, normalized(Point3{0.0f, 1.0f, -1.0f}), normalized(Point3{0.0f, 1.0f, 1.0f})});

    displayManager->startMainLoop();

    return 0;


    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        SDL_SetRenderDrawColor(displayManager->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(displayManager->getRenderer());

        // draw axis
        SDL_SetRenderDrawColor(displayManager->getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(displayManager->getRenderer(), 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
        SDL_SetRenderDrawColor(displayManager->getRenderer(), 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLine(displayManager->getRenderer(), SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);

        renderer.render(tCube);


        SDL_RenderPresent(displayManager->getRenderer());

        tCube.getPosition().z -= 0.5f;
    }

    return 0;
}

