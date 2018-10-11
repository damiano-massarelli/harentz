#include <iostream>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include "Point3.h"
#include "PointLight.h"
#include <memory>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

bool init();

void close();

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;

int main(int argc, char* argv[])
{
    init();

    Renderer renderer(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 200);
    renderer.setLight(std::make_unique<PointLight>(Point3{0.0f, -100.0f, 0.0f}, SDL_Color{255, 0, 0, 255}));


    Shape sCube(std::vector<Point3>{ Point3{-20.0f, -20.0f, 20.0f}, Point3{20.0f, -20.0f, 20.0f},
                                     Point3{20.0f, 20.0f, 20.0f}, Point3{-20.0f, 20.0f, 20.0f},
                                     Point3{20.0f, -20.0f, 10.0f}, Point3{20.0f, 20.0f, 10.0f},
                                     Point3{-20.0f, -20.0f, 10.0f}, Point3{-20.0f, 20.0f, 10.0f}},
                std::vector<int>{3, 2, 1, 0});/*, // front face
                                1, 4, 2, 5, // right face
                                0, 3, 7, 6});*/ // left face

    Transform tCube(sCube, Point3{00.0f, 000.0f, -300.0f});


    //Point3 points[]{{{300, 300}, 0.001f}, {{350, 300}, 0.0005f}, {{350, 350}, 0.0005f}, {{300, 350}, 0.001f},  {{300, 300}, 0.001f}};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        renderer.render(tCube);


        //render3d(gRenderer, &vPoint, points, 5);

        SDL_RenderPresent(gRenderer);

        tCube.getPosition().z += 0.05f;
        //std::cout << tCube.getPosition().z << "\n";
        tCube.getPosition().x += 0.0f;
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
