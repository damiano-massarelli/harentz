#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <SDL2/SDL.h>
#include <memory>
#include "EventListener.h"
#include "EventManager.h"
#include "Scene.h"
#include "TransitionManager.h"

class DisplayManager : public EventListener
{
    private:
        static std::unique_ptr<DisplayManager> instance;

        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        TransitionManager* m_transitionManager = nullptr;

        EventManager* m_eventManager = nullptr;
        Scene* m_currentScene = nullptr;

        bool m_quit{false}; ///< true if should quit main loop

        DisplayManager(const std::string& title, int displayWidth, int displayHeight);


    public:
        static DisplayManager* create(const std::string& title, int displayWidth, int displayHeight);
        static DisplayManager* getInstance();

        static int screenWidth();
        static int screenHeight();

        DisplayManager(const DisplayManager& dispManager) = delete;
        DisplayManager& operator=(const DisplayManager& dispManager) = delete;

        void startMainLoop();

        SDL_Window* getWindow() const;

        SDL_Renderer* getRenderer() const;

        EventManager& getEventManager();

        void setCurrentScene(Scene* scene);

        void quit();

        virtual void onEvent(SDL_Event event) override;

        virtual ~DisplayManager();

};

#endif // DISPLAYMANAGER_H
