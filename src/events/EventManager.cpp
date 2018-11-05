#include "EventManager.h"
#include <algorithm>
#include <iostream>

// Static variable definition
const SDL_EventType EventManager::ENTER_FRAME_EVENT = static_cast<SDL_EventType>(SDL_RegisterEvents(1));

EventManager::EventManager()
{
    //ctor
}

std::unique_ptr<EventListenerCrumb> EventManager::addListenerFor(SDL_EventType event, EventListener* listener, bool wantCrumb)
{
    if (m_event2listeners.count(event))
        m_event2listeners.at(event).push_back(listener);
    else
        m_event2listeners.insert(std::make_pair(event, std::vector<EventListener*>{listener}));

    std::unique_ptr<EventListenerCrumb> crumb;
    if (wantCrumb)
        crumb = std::make_unique<EventListenerCrumb>(event, listener);

    return crumb;
}

void EventManager::removeListenerFor(SDL_EventType event, EventListener* listener)
{
    if (m_event2listeners.count(event)) {
        std::cout << "deleted listener " << listener << " for event " << event <<"\n";
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

void EventManager::pushEnterFrameEvent() const
{
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = ENTER_FRAME_EVENT;
    event.user.code = 0;
    event.user.data1 = 0;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}


EventManager::~EventManager()
{
    //dtor
}
