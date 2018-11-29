#include "TransitionManager.h"

void TransitionManager::addTransition(const std::string& tag, std::shared_ptr<AbstractTransition>& transition)
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
        ++it;
    }
}
