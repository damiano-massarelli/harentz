#ifndef TRANSITIONS_H
#define TRANSITIONS_H
#include <memory>
#include <map>
#include "AbstractTransition.h"
#include "EventListener.h"
#include "EventListenerCrumb.h"

class TransitionManager : public EventListener
{

    private:
        std::unique_ptr<EventListenerCrumb> m_enterFrameCrumb; ///< crumb for the enterframe event
        std::multimap<std::string, std::shared_ptr<AbstractTransition>> m_tag2transition; ///< list of all the transitions for a tag

    public:
        TransitionManager();

        void addTransition(const std::string& tag, std::shared_ptr<AbstractTransition>& transition);

        void cancelAll();

        void cancelByTag(const std::string& tag);

        ~TransitionManager();
};

#endif // TRANSITIONS_H
