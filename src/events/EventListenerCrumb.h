#ifndef EVENTLISTENERCRUMB_H
#define EVENTLISTENERCRUMB_H
#include "EventListener.h"
#include <SDL.h>
#include <vector>

/** \brief event listener crumbs automatically unregister an event listener when it is deleted.
  * EventListenerCrumb objects should be returned as unique_ptr and the EventListener receiving
  * it should store in a member variable so that when the EventListener is destroyed the EventListenerCrumb
  * can unregister it
  */
class EventListenerCrumb
{
    private:
        std::vector<SDL_EventType> m_eventTypes;
        EventListener* m_listener;

    public:
        EventListenerCrumb(SDL_EventType eventType, EventListener* listener);

        void unregister();

        EventListener* getListener();

        void addEvent(SDL_EventType eventType);

        virtual ~EventListenerCrumb();

};

#endif // EVENTLISTENERCRUMB_H
