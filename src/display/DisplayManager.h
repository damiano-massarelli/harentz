#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <SDL2/SDL.h>
#include <memory>
#include "EventListener.h"

/* In order to avoid cyclic dependency: DisplayManager's declaration (header) is defined
using EventManager whose declaration uses EventListenerCrumb that in turn imports DisplayManager.
Hence, while DisplayManager is being declared its complete declaration is already necessary
to compile EventListenerCrumb and this is impossible. So, DisplayManager header uses a forward declaration
for EventManager and its implementation actually uses its definition */
class EventManager;
class Scene;

class DisplayManager : public EventListener
{
    private:
        static std::unique_ptr<DisplayManager> instance;

        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;

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
