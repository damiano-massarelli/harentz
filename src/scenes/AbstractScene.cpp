#include "AbstractScene.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include <SDL2/SDL.h>

AbstractScene::AbstractScene()
{
    // register the enter frame event
    m_eventCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);
}

void AbstractScene::onEvent(SDL_Event e)
{
    if (e.type == EventManager::ENTER_FRAME_EVENT) {
        for (auto& renderable : m_renderList)
            //renderable->render();
            ;
    }
}

void AbstractScene::onCreate(SDL_Renderer* renderer)
{

}

void AbstractScene::add(Renderable* renderable)
{

}

void AbstractScene::remove(Renderable* renderable)
{

}


AbstractScene::~AbstractScene()
{
    //dtor
}
