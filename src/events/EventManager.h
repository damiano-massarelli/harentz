#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <map>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "EventListener.h"
#include "EventListenerCrumb.h"

class EventManager
{
    private:
        std::map<SDL_EventType, std::vector<EventListener*>> m_event2listeners;

    public:
        static const SDL_EventType ENTER_FRAME_EVENT;

        EventManager();

        std::unique_ptr<EventListenerCrumb> addListenerFor(SDL_EventType event, EventListener* listener, bool wantCrumb = false);

        void removeListenerFor(SDL_EventType event, EventListener* listener);

        void dispatchEvents() const;

        void pushEnterFrameEvent(Uint32* deltaMillis) const;

        virtual ~EventManager();
};

#endif // EVENTMANAGER_H
