#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <map>
#include <vector>
#include <memory>
#include <SDL.h>
#include "EventListener.h"
#include "EventListenerCrumb.h"

class EventManager
{
    private:
        /** m_event2listeners cannot be modified while iterating it. Two other support
          * maps are used to temporarily store values to add and remove */
        std::map<SDL_EventType, std::vector<EventListener*>> m_event2listeners;
        std::map<SDL_EventType, std::vector<EventListener*>> m_toAdd;
        std::map<SDL_EventType, std::vector<EventListener*>> m_toRemove;

    public:
        static const SDL_EventType ENTER_FRAME_EVENT;

        EventManager();

        std::unique_ptr<EventListenerCrumb> addListenerFor(SDL_EventType event, EventListener* listener, bool wantCrumb = false);

        /** \brief Same as addListenerFor but adds the listener to an existing crumb */
        void addListenerFor(SDL_EventType event, EventListenerCrumb* existingCrumb);

        void removeListenerFor(SDL_EventType event, EventListener* listener);

        /** \brief dispatches all the events from the event queue */
        void dispatchEvents();

        /** \brief dispatches a certain event to all the listeners registered for that event */
        void dispatchToListeners(SDL_Event& event);

        void pushEnterFrameEvent(Uint32* deltaMillis) const;

        virtual ~EventManager();
};

#endif // EVENTMANAGER_H
