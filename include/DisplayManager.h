#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <SDL2/SDL.h>
#include <memory>

class DisplayManager
{
    private:
        SDL_Window* m_window = nullptr; ///< Window before renderer so that renderer is destroyed before window
        SDL_Renderer* m_renderer = nullptr;

    public:
        DisplayManager(const std::string& title, int displayWidth, int displayHeight);

        DisplayManager(const DisplayManager& dispManager) = delete;
        DisplayManager& operator=(const DisplayManager& dispManager) = delete;

        SDL_Window* getWindow() const;

        SDL_Renderer* getRenderer() const;

        void quit();

        virtual ~DisplayManager();

};

#endif // DISPLAYMANAGER_H
