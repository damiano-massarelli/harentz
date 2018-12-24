#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <SDL.h>
#include <SDL_gpu.h>
#include <memory>
#include "EventListener.h"
#include "EventManager.h"
#include "Scene.h"
#include "TransitionManager.h"

class DisplayManager : public EventListener
{
    private:
        static constexpr float FPS_CAP = 60.0f;
        static std::unique_ptr<DisplayManager> instance;

        GPU_Target* m_screen;
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

        GPU_Target* getScreen() const;

        EventManager& getEventManager();

        TransitionManager& getTransitionManager();

        void setCurrentScene(Scene* scene);

        void quit();

        virtual void onEvent(SDL_Event event) override;

        virtual ~DisplayManager();
};

#endif // DISPLAYMANAGER_H
