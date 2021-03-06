#include "Scene.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include "AbstractRenderable.h"
#include <algorithm>
#include <SDL.h>

Scene::Scene()
{

}

void Scene::onEvent(SDL_Event event)
{
    /* Handles the different events that can occur while a scene is shown */
    if (event.type == EventManager::ENTER_FRAME_EVENT)
        onEnterFrame(event);
    if (event.type == SDL_APP_WILLENTERBACKGROUND)
        onPause(EventStatus::WILL);
    if (event.type == SDL_APP_DIDENTERBACKGROUND)
        onPause(EventStatus::DID);
    if (event.type == SDL_APP_WILLENTERFOREGROUND)
        onResume(EventStatus::WILL);
    if (event.type == SDL_APP_DIDENTERFOREGROUND)
        onResume(EventStatus::DID);
}

void Scene::onEnterFrame(SDL_Event& e)
{
    if (e.type == EventManager::ENTER_FRAME_EVENT) {
        // Clears the screen
        GPU_ClearColor(m_screen, m_bgColor);
        for (auto& renderable : m_renderList)
            renderable->render();

        onRenderingComplete();

        for (auto& uiRenderable : m_uiRenderList)
            uiRenderable->render();

        // Render
        GPU_Flip(m_screen);
    }
}

void Scene::add(AbstractRenderable* renderable, bool isUiElement)
{
    renderable->setScene(this);
    if (isUiElement)
        m_uiRenderList.push_back(renderable);
    else
        m_renderList.push_back(renderable);
}

void Scene::remove(AbstractRenderable* renderable)
{
    m_renderList.erase(std::remove(m_renderList.begin(), m_renderList.end(), renderable), m_renderList.end());
    m_uiRenderList.erase(std::remove(m_uiRenderList.begin(), m_uiRenderList.end(), renderable), m_uiRenderList.end());
}

void Scene::onShow(GPU_Target* screen)
{
    m_screen = screen;

    // register the enter frame event and the pause/resume events
    EventManager& evtManager = DisplayManager::getInstance()->getEventManager();
    m_enterFrameCrumb = evtManager.addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);

    // Pause
    m_willEnterBgCrumb = evtManager.addListenerFor(SDL_APP_WILLENTERBACKGROUND, this, true);
    m_didEnterBgCrumb = evtManager.addListenerFor(SDL_APP_DIDENTERBACKGROUND, this, true);

    // Foreground
    m_willEnterFgCrumb = evtManager.addListenerFor(SDL_APP_WILLENTERFOREGROUND, this, true);
    m_didEnterFgCrumb = evtManager.addListenerFor(SDL_APP_DIDENTERFOREGROUND, this, true);
}

void Scene::setBgColor(const SDL_Color& color)
{
    m_bgColor = color;
}

GPU_Target* Scene::getScreen()
{
    return m_screen;
}


Scene::~Scene()
{

}
