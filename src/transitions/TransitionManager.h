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

        /** \brief After this method is called transitions are updated as frames go by */
        void startUpdatingTransitions();

        /** \brief adds a transition with a given tag name
          * \param tag the name of the tag
          * \param transition the transition to execute */
        void addTransition(const std::string& tag, std::shared_ptr<AbstractTransition> transition);

        /** \brief cancels all the existing transitions */
        void cancelAll();

        /** \brief cancels all the transitions with that tag */
        void cancelByTag(const std::string& tag);

        void onEvent(SDL_Event e) override;

        ~TransitionManager();
};

#endif // TRANSITIONS_H
