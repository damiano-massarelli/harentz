#include "DisplayManager.h"
#include "TransitionManager.h"
#include "Scene.h"
#include "Timer.h"
#include <iostream>

// Static variable definition.
std::unique_ptr<DisplayManager> DisplayManager::instance;

DisplayManager* DisplayManager::create(const std::string& title, int displayWidth, int displayHeight, bool fitScreen) {
    if (!instance)
        instance = std::unique_ptr<DisplayManager>(new DisplayManager{title, displayWidth, displayHeight, fitScreen});

    return instance.get();
}

DisplayManager* DisplayManager::getInstance()
{
    return instance.get();
}

int DisplayManager::screenWidth()
{
    return getInstance()->m_screen->w;
}

int DisplayManager::screenHeight()
{
    return getInstance()->m_screen->h;
}


DisplayManager::DisplayManager(const std::string& title, int displayWidth, int displayHeight, bool fitScreen)
{
    SDL_Init(SDL_INIT_VIDEO); // so that it is possible to call SDL_GetDisplayMode
    SDL_DisplayMode mode;
    if (SDL_GetDisplayMode(0, 0, &mode) == 0 && fitScreen) {
        displayHeight = static_cast<int>((mode.h / static_cast<float>(mode.w)) * displayWidth);
    }

    GPU_SetPreInitFlags(GPU_INIT_ENABLE_VSYNC);
    m_screen = GPU_Init(displayWidth, displayHeight, GPU_DEFAULT_INIT_FLAGS);

    m_eventManager = new EventManager{};
    m_transitionManager = new TransitionManager{};
}

void DisplayManager::startMainLoop()
{
    m_eventManager->addListenerFor(SDL_QUIT, this);
    Timer elapsedTimer;
    elapsedTimer.start();

    Uint32 renderDelta = 0; ///< time elapsed to render
    Uint32 actualDelta = 0; ///< time elapsed taking into account fps capping
    float lastTime = elapsedTimer.get();

    m_transitionManager->startUpdatingTransitions(); // Transitions are now updated as frames go by

    while (!m_quit) {
        // Delta time calculations: elapsed time
        lastTime = elapsedTimer.get();

        m_eventManager->pushEnterFrameEvent(&actualDelta);
        m_eventManager->dispatchEvents();

        renderDelta = elapsedTimer.get() - lastTime;
        // it is ok to enable this on pc but it creates weird effects on android
        //float sleepTime = (1000.0f/FPS_CAP) - renderDelta;
        //if (sleepTime > 0) SDL_Delay(sleepTime);

        /* changes current scene if needed (m_nextScene != nullptr) */
        if (m_nextScene != nullptr) {
            if (m_currentScene != nullptr) {
                m_currentScene->onRemove();

                delete m_currentScene;
            }
            m_currentScene = m_nextScene;
            if (m_currentScene != nullptr)
                m_currentScene->onShow(m_screen);

            m_nextScene = nullptr;
        }
        actualDelta = elapsedTimer.get() - lastTime;
     }
}

// Override from EventListener
void DisplayManager::onEvent(SDL_Event event) {
    if (event.type == SDL_QUIT)
        m_quit = true;
}

GPU_Target* DisplayManager::getScreen() const
{
    return m_screen;
}

void DisplayManager::setCurrentScene(Scene* scene)
{
    m_nextScene = scene;
}

Scene* DisplayManager::getCurrentScene()
{
    return m_currentScene;
}

void DisplayManager::quit()
{
    setCurrentScene(nullptr);

    delete m_transitionManager;
    m_transitionManager = nullptr;

    delete m_eventManager;
    m_eventManager = nullptr;

    GPU_Quit();
}

EventManager& DisplayManager::getEventManager()
{
    return *m_eventManager;
}

TransitionManager& DisplayManager::getTransitionManager()
{
    return *m_transitionManager;
}

DisplayManager::~DisplayManager()
{
    quit();
}
