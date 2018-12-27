#include "Scene.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include "AbstractRenderable.h"
#include <algorithm>
#include <SDL.h>

Scene::Scene()
{

}

void Scene::onEvent(SDL_Event e)
{
    // Clears the screen
    GPU_ClearColor(m_screen, m_bgColor);

    if (e.type == EventManager::ENTER_FRAME_EVENT) {
        for (auto& renderable : m_renderList)
            renderable->render();
    }

    onRenderingComplete();

    // Render
    GPU_Flip(m_screen);
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

void Scene::onShow(GPU_Target* screen)
{
    m_screen = screen;

    // register the enter frame event
    m_eventCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);
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
