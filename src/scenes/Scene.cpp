#include "Scene.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include "AbstractRenderable.h"
#include <algorithm>
#include <SDL2/SDL.h>

Scene::Scene()
{

}

void Scene::onEvent(SDL_Event e)
{
    // Clears the screen
    SDL_SetRenderDrawColor(m_sdlRenderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, 255);
    SDL_RenderClear(m_sdlRenderer);

    if (e.type == EventManager::ENTER_FRAME_EVENT) {
        for (auto& renderable : m_renderList)
            renderable->render();
    }

    onRenderingComplete();

    // Render
    SDL_RenderPresent(m_sdlRenderer);
}

void Scene::add(AbstractRenderable* renderable)
{
    renderable->setScene(this);
    m_renderList.push_back(renderable);
}

void Scene::remove(AbstractRenderable* renderable)
{
    m_renderList.erase(std::remove(m_renderList.begin(), m_renderList.end(), renderable), m_renderList.end());
}

void Scene::onShow(SDL_Window* window, SDL_Renderer* renderer)
{
    m_sdlRenderer = renderer;

    // register the enter frame event
    m_eventCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);
}

void Scene::setBgColor(const SDL_Color& color)
{
    m_bgColor = color;
}

SDL_Renderer* Scene::getSDLRenderer()
{
    return m_sdlRenderer;
}


Scene::~Scene()
{

}
