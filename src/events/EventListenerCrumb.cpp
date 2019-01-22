#include "EventListenerCrumb.h"
#include "EventManager.h"
#include "DisplayManager.h"

EventListenerCrumb::EventListenerCrumb(SDL_EventType eventType, EventListener* listener) : m_eventTypes{eventType}, m_listener{listener}
{

}

void EventListenerCrumb::unregister()
{
    EventManager& eventManager = DisplayManager::getInstance()->getEventManager();
    for (auto eventType : m_eventTypes)
        eventManager.removeListenerFor(eventType, m_listener);
}

void EventListenerCrumb::addEvent(SDL_EventType eventType)
{
    m_eventTypes.push_back(eventType);
}

EventListener* EventListenerCrumb::getListener()
{
    return m_listener;
}

EventListenerCrumb::~EventListenerCrumb()
{
    unregister();
}
