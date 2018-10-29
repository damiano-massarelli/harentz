#include "EventListenerCrumb.h"
#include "EventManager.h"

EventListenerCrumb::EventListenerCrumb(SDL_EventType eventType, EventListener* listener) : m_eventType{eventType}, m_listener{listener}
{

}

void EventListenerCrumb::unregister()
{
    DisplayManager::getInstance()->getEventManager().removeListenerFor(m_eventType, m_listener);
}


EventListenerCrumb::~EventListenerCrumb()
{
    unregister();
}
