#include "DisplayManager.h"
#include "TransitionManager.h"
#include "Scene.h"
#include <iostream>

// Static variable definition.
std::unique_ptr<DisplayManager> DisplayManager::instance;

DisplayManager* DisplayManager::create(const std::string& title, int displayWidth, int displayHeight) {
    if (!instance)
        instance = std::unique_ptr<DisplayManager>(new DisplayManager{title, displayWidth, displayHeight});

    return instance.get();
}

DisplayManager* DisplayManager::getInstance()
{
    return instance.get();
}

int DisplayManager::screenWidth()
{
    int width = -1;
    SDL_GetWindowSize(getInstance()->getWindow(), &width, nullptr);
    return width;
}

int DisplayManager::screenHeight()
{
    int height = -1;
    SDL_GetWindowSize(getInstance()->getWindow(), nullptr, &height);
    return height;
}


DisplayManager::DisplayManager(const std::string& title, int displayWidth, int displayHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << SDL_GetError();

    SDL_Window *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayWidth, displayHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cout << SDL_GetError() << "\n";

    m_window = window;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == nullptr)
        std::cout << SDL_GetError() << "\n";

    m_renderer = renderer;
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_eventManager = new EventManager{};
    m_transitionManager = new TransitionManager{};
}

void DisplayManager::startMainLoop()
{
    m_eventManager->addListenerFor(SDL_QUIT, this);
    Uint32 renderDelta = 0; ///< time elapsed to render
    Uint32 actualDelta = 0; ///< time elapsed taking into account fps capping
    float lastTime = SDL_GetTicks();

    m_transitionManager->startUpdatingTransitions(); // Transitions are now updated as frames go by

    while (!m_quit) {
        // Delta time calculations: elapsed time
        lastTime = SDL_GetTicks();

        m_eventManager->pushEnterFrameEvent(&actualDelta);
        m_eventManager->dispatchEvents();

        renderDelta = SDL_GetTicks() - lastTime;
        float sleepTime = (1000.0f/FPS_CAP) - renderDelta;
        if (sleepTime > 0) SDL_Delay(sleepTime);

        actualDelta = SDL_GetTicks() - lastTime;
     }
}

// Override from EventListener
void DisplayManager::onEvent(SDL_Event event) {
    if (event.type == SDL_QUIT)
        quit();
}

SDL_Window* DisplayManager::getWindow() const
{
    return m_window;
}

SDL_Renderer* DisplayManager::getRenderer() const
{
    return m_renderer;
}

void DisplayManager::setCurrentScene(Scene* scene)
{
    if (m_currentScene != nullptr) {
        m_currentScene->onRemove();
        delete m_currentScene;
    }
    m_currentScene = scene;
    if (scene != nullptr)
        m_currentScene->onShow(m_window, m_renderer);
}


void DisplayManager::quit()
{
    m_quit = true; // quit main loop

    setCurrentScene(nullptr);

    delete m_transitionManager;
    m_transitionManager = nullptr;

    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    delete m_eventManager;
    m_eventManager = nullptr;

    SDL_Quit();
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
