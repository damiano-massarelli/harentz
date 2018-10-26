#include "EventManager.h"
#include <algorithm>

EventManager::EventManager()
{
    //ctor
}

void EventManager::addListenerFor(SDL_EventType event, EventListener* listener)
{
    if (m_event2listeners.count(event))
        m_event2listeners.at(event).push_back(listener);
    else
        m_event2listeners.insert(std::make_pair(event, std::vector<EventListener*>{listener}));
}

void EventManager::removeListenerFor(SDL_EventType event, EventListener* listener)
{
    if (m_event2listeners.count(event)) {
        std::vector<EventListener*>& listeners = m_event2listeners.at(event);
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    }
}

void EventManager::dispatchEvents() const
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (m_event2listeners.count(eventType)) {
            for (auto& listener : m_event2listeners.at(eventType))
                listener->onEvent(event);
        }
    }
}


EventManager::~EventManager()
{
    //dtor
}
