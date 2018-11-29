#include "AbstractTransition.h"
#include "DisplayManager.h"
#include "EventManager.h"

AbstractTransition::AbstractTransition(float durationMS) : m_totalDuration{durationMS}
{

}

void AbstractTransition::onEnterFrame(float elapsed)
{
    //  another frame passed, increase total elapsed time
    m_elapsed += elapsed;
    float f = m_elapsed/m_totalDuration;
    if (f >= 1.0f) { // this transition is over, remove it
        f = 1.0f;
        cancel();
    }
    onUpdate(f);
}

void AbstractTransition::cancel()
{
    m_cancelled = true;
}

AbstractTransition::~AbstractTransition()
{

}
