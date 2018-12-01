#include "TransitionManager.h"
#include "DisplayManager.h"

TransitionManager::TransitionManager()
{
    //m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);
}

void TransitionManager::startUpdatingTransitions()
{
    /* This command cannot be placed in the constructor as the TransitionManager is created
    inside the constructor of display manager. Hence, getInstance would return null */
    m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);
}


void TransitionManager::addTransition(const std::string& tag, std::shared_ptr<AbstractTransition> transition)
{
    m_tag2transition.insert(std::make_pair(tag, transition));
}

void TransitionManager::cancelAll()
{
    // stops all the transitions and removes them
    for (auto it = m_tag2transition.begin(); it != m_tag2transition.end(); ) {
        it->second->cancel();
        it = m_tag2transition.erase(it);
    }

}

void TransitionManager::cancelByTag(const std::string& tag)
{
    auto elementsRange = m_tag2transition.equal_range(tag); // range of elements for the given tag
    // iterates through the elements canceling and erasing those whose name is equal to tag
    for (auto it = elementsRange.first; it != elementsRange.second; ) {
        if (it->first == tag) {
            it->second->cancel();
            it = m_tag2transition.erase(it);
        }
        else
            ++it;
    }
}

void TransitionManager::onEvent(SDL_Event e)
{
    float elapsed = *(static_cast<Uint32*>(e.user.data1));
    for (auto it = m_tag2transition.begin(); it != m_tag2transition.end(); ) {
        it->second->onEnterFrame(elapsed);
        if (it->second->isCancelled()) // This transitions has expired, remove it
            it = m_tag2transition.erase(it);
        else
            ++it;
    }
}

TransitionManager::~TransitionManager()
{

}
