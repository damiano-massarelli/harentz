#ifndef ABSTRACTTRANSITION_H
#define ABSTRACTTRANSITION_H
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include <memory>

class AbstractTransition
{
    private:
        float m_elapsed{0.0f}; ///< total elapsed time from the creation of this transition
        float m_totalDuration{500.0f}; ///< total desired duration for this transition

        bool m_cancelled{false};

    protected:
        AbstractTransition(float durationMS);

    public:
        void onEnterFrame(float elapsed); // called by Transitions

        virtual void onUpdate(float f) const = 0;

        void cancel();

        bool isCancelled();

        virtual ~AbstractTransition();
};

#endif // ABSTRACTTRANSITION_H
