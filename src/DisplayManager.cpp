#include "DisplayManager.h"
#include <iostream>

DisplayManager::DisplayManager(const std::string& title, int displayWidth, int displayHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << SDL_GetError();

    SDL_Window *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayWidth, displayHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cout << SDL_GetError() << "\n";

    m_window = window;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
        std::cout << SDL_GetError() << "\n";

    m_renderer = renderer;
}

SDL_Window* DisplayManager::getWindow() const
{
    return m_window;
}

SDL_Renderer* DisplayManager::getRenderer() const
{
    return m_renderer;
}

void DisplayManager::quit()
{
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
}


DisplayManager::~DisplayManager()
{
    quit();
}
