#ifndef TIMER_H
#define TIMER_H
#include <SDL.h>
#include <memory>
#include "EventListener.h"
#include "EventListenerCrumb.h"

class Timer : public EventListener
{
    private:
        std::unique_ptr<EventListenerCrumb> m_willEnterBgCrumb;
        std::unique_ptr<EventListenerCrumb> m_willEnterFgCrumb;

        bool m_started = false;
        bool m_paused = false;

        Uint32 m_startTime = 0;
        Uint32 m_pauseStartTime = 0;

    public:
        Timer();

        void start();

        void pause();

        void resume();

        void stop();

        Uint32 get();

        virtual void onEvent(SDL_Event e) override;

        virtual ~Timer();
};

#endif // TIMER_H
