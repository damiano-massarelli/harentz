#include "Timer.h"
#include "DisplayManager.h"

Timer::Timer()
{
    m_willEnterBgCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_APP_WILLENTERBACKGROUND, this, true);
    m_willEnterFgCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(SDL_APP_WILLENTERFOREGROUND, this, true);
}

void Timer::start()
{
    if (!m_started) {
        m_started = true;
        m_startTime = SDL_GetTicks();
    }
}

void Timer::pause()
{
    if (!m_paused) {
        m_paused = true;
        m_pauseStartTime = SDL_GetTicks();
    }
}

void Timer::resume()
{
    if (m_paused) {
        Uint32 elapsedPaused = SDL_GetTicks() - m_pauseStartTime;
        m_startTime += elapsedPaused;
        m_paused = false;
    }
}

void Timer::stop()
{
    if (m_started)
        m_started = false;
}

Uint32 Timer::get()
{
    if (!m_started)
        return -1;

    if (m_paused)
        return SDL_GetTicks() - (m_startTime + SDL_GetTicks() - m_pauseStartTime);

    return SDL_GetTicks() - m_startTime;
}

void Timer::onEvent(SDL_Event e)
{
    if (e.type == SDL_APP_WILLENTERBACKGROUND)
        pause();
    else if (e.type == SDL_APP_WILLENTERFOREGROUND)
        resume();
}

Timer::~Timer()
{
    //dtor
}
