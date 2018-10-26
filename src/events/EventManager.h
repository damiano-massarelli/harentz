#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include "EventListener.h"

class EventManager
{
    private:
        std::map<SDL_EventType, std::vector<EventListener*>> m_event2listeners;

    public:
        EventManager();

        void addListenerFor(SDL_EventType event, EventListener* listener);

        void removeListenerFor(SDL_EventType event, EventListener* listener);

        void dispatchEvents() const;

        virtual ~EventManager();


};

#endif // EVENTMANAGER_H
