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

        /* Scenes cannot be swapped when setCurrentScene is called
        we must wait until the current frame is over. If this variable
        contain a scene it will become the next scene once the current frame is over */
        Scene* m_nextScene = nullptr;

        bool m_quit{false}; ///< true if should quit main loop

        DisplayManager(const std::string& title, int displayWidth, int displayHeight, bool fitScreen);


    public:
        static DisplayManager* create(const std::string& title, int displayWidth, int displayHeight, bool fitScreen = false);
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

        Scene* getCurrentScene();

        void quit();

        virtual void onEvent(SDL_Event event) override;

        virtual ~DisplayManager();
};

#endif // DISPLAYMANAGER_H
