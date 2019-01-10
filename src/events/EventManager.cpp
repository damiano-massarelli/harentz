#include "EventManager.h"
#include <algorithm>

// Static variable definition
const SDL_EventType EventManager::ENTER_FRAME_EVENT = static_cast<SDL_EventType>(SDL_RegisterEvents(1));

EventManager::EventManager()
{
    //ctor
}

std::unique_ptr<EventListenerCrumb> EventManager::addListenerFor(SDL_EventType event, EventListener* listener, bool wantCrumb)
{
    if (m_toAdd.count(event))
        m_toAdd.at(event).push_back(listener);
    else
        m_toAdd.insert(std::make_pair(event, std::vector<EventListener*>{listener}));

    std::unique_ptr<EventListenerCrumb> crumb;
    if (wantCrumb)
        crumb = std::make_unique<EventListenerCrumb>(event, listener);

    return crumb;
}

void EventManager::removeListenerFor(SDL_EventType event, EventListener* listener)
{
    if (m_toRemove.count(event))
        m_toRemove.at(event).push_back(listener);
    else
        m_toRemove.insert(std::make_pair(event, std::vector<EventListener*>{listener}));
}

void EventManager::dispatchEvents()
{
    /* Dispatches all events */
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (m_event2listeners.count(eventType)) {
            for (auto& listener : m_event2listeners.at(eventType)) {
                /* Skip a listener if it is removed */
                if (m_toRemove.count(eventType)) {
                    auto& removed = m_toRemove.at(eventType);
                    if (std::find(removed.begin(), removed.end(), listener) != removed.end())
                        continue;
                }

                listener->onEvent(event);
            }
        }
    }

    /* Adds the listeners */
    for (auto it = m_toAdd.begin(); it != m_toAdd.end(); ++it)
        if (m_event2listeners.count(it->first)) // already has some listeners for this event, append
            m_event2listeners.at(it->first).insert(m_event2listeners.at(it->first).end(), it->second.begin(), it->second.end());
        else // No listener for this event, just copy
            m_event2listeners.insert(std::make_pair(it->first, it->second));

    /* Removes the listeners */
    for (auto it = m_toRemove.begin(); it != m_toRemove.end(); ++it) {
        if (m_event2listeners.count(it->first)) {
            auto& listeners = m_event2listeners.at(it->first);
            listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [&it](auto& elem){
                                           return std::find(it->second.begin(), it->second.end(), elem) != it->second.end();} ), listeners.end());
        }

    }

    m_toRemove.clear();
    m_toAdd.clear();

}

void EventManager::pushEnterFrameEvent(Uint32* deltaMillis) const
{
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = ENTER_FRAME_EVENT;
    event.user.code = 0;
    event.user.data1 = static_cast<void*>(deltaMillis);
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}


EventManager::~EventManager()
{
    //dtor
}
