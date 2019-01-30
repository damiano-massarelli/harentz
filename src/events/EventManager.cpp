#include "EventManager.h"
#include <algorithm>

// Static variable definition
const SDL_EventType EventManager::ENTER_FRAME_EVENT = static_cast<SDL_EventType>(SDL_RegisterEvents(1));

/** \brief handles events as they happen instead of putting them in the event queue
  *
  * Some events must be handled immediately. For instance app events cannot wait an entire frame
  * to be handled as the OS might pause/terminate the app before the frame is completely rendered */
static int eventFilterFunction(void* eventManager, SDL_Event* event) {
    if (event->type == SDL_APP_WILLENTERBACKGROUND || event->type == SDL_APP_DIDENTERBACKGROUND) {
        EventManager* evtManager = static_cast<EventManager*>(eventManager);
        evtManager->dispatchToListeners(*event);
    }

    /* simulates pause and resume when p and r are pressed on the keyboard. Only for debugging */
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_p)
            event->type = SDL_APP_WILLENTERBACKGROUND;
        if (event->key.keysym.sym == SDLK_r)
            event->type = SDL_APP_WILLENTERFOREGROUND;
    } else if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_p)
            event->type = SDL_APP_DIDENTERBACKGROUND;
        if (event->key.keysym.sym == SDLK_r)
            event->type = SDL_APP_DIDENTERFOREGROUND;
    }

    return 1;
}


EventManager::EventManager()
{
    SDL_SetEventFilter(eventFilterFunction, this);
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

void EventManager::addListenerFor(SDL_EventType event, EventListenerCrumb* existingCrumb)
{
    existingCrumb->addEvent(event);
    addListenerFor(event, existingCrumb->getListener(), false);
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
        dispatchToListeners(event);
    }

    /* Adds the listeners */
    for (auto it = m_toAdd.begin(); it != m_toAdd.end(); ++it)
        if (m_event2listeners.count(it->first)) // already has some listeners for this event, append
            m_event2listeners.at(it->first).insert(m_event2listeners.at(it->first).end(), it->second.begin(), it->second.end());
        else // No listener for this event, just copy
            m_event2listeners.insert(std::make_pair(it->first, it->second));

    m_toAdd.clear();
}

void EventManager::dispatchToListeners(SDL_Event& event)
{
    // Event type
    SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
    if (m_event2listeners.count(eventType)) {
        // List of listeners for this type of event
        auto& listeners = m_event2listeners[eventType];
        for (auto it = listeners.begin(); it != listeners.end(); ) {
            /* Skip a listener if it is removed and removes it */
            if (m_toRemove.count(eventType)) {
                auto& removed = m_toRemove[eventType];
                const auto& listenerToRemove = std::find(removed.begin(), removed.end(), *it);
                if (listenerToRemove != removed.end()) {
                    removed.erase(listenerToRemove);
                    it = listeners.erase(it);
                    continue;
                }
            }

            (*it)->onEvent(event);
            ++it;
        }
    }
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

}
