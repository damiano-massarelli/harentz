#include <iostream>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include "Point3.h"
#include "PointLight.h"
#include <memory>
#include "BackfaceCulling.h"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SCREEN_Z = 0;
constexpr int PROJECTION_POINT_Z = -500;

bool init();

void close();

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;

int main(int argc, char* argv[])
{
    init();

    Renderer renderer(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_Z, PROJECTION_POINT_Z);

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


    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        // draw axis
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);

        renderer.render(tCube);


        SDL_RenderPresent(gRenderer);

        tCube.getPosition().z -= 0.05f;
    }

    close();

    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError();
        return false;
    }

    gWindow = SDL_CreateWindow("Harentz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << SDL_GetError();
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << SDL_GetError();
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}
