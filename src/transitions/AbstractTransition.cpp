#include "AbstractTransition.h"
#include "DisplayManager.h"
#include "EventManager.h"

AbstractTransition::AbstractTransition(float durationMS, std::function<void()> onComplete)
            : m_totalDuration{durationMS}, m_onCompleteCallback{onComplete}
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

    /* If the transitions is complete, onComplete callback is called */
    if (isCancelled() && m_onCompleteCallback != nullptr)
        m_onCompleteCallback();

}

void AbstractTransition::cancel()
{
    m_cancelled = true;
}

bool AbstractTransition::isCancelled()
{
    return m_cancelled;
}

AbstractTransition::~AbstractTransition()
{

}
