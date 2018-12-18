#ifndef LINEARTRANSITION_H
#define LINEARTRANSITION_H
#include "AbstractTransition.h"
#include "TransitionManager.h"
#include "DisplayManager.h"
#include <memory>
#include <functional>

template <class T>
class LinearTransition : public AbstractTransition
{
    private:
        T m_initialValue{0.0f};
        T m_finalValue{0.0f};
        std::function<void(T)> m_updater;

    protected:
        // Moves the lambda so that it can store unique_ptrs without problems
        LinearTransition(T initialValue, T finalValue, std::function<void(T)> updater, float durationMS, std::function<void()> onComplete) :
            AbstractTransition{durationMS, onComplete}, m_initialValue{initialValue}, m_finalValue{finalValue}, m_updater{updater}  {}

    public:
        /** \brief Creates and returns a LinearTransition
          * \param initValue the initial value for this transition
          * \param finalValue the final value for the transition
          * \param updater a function called every time the transition is updated
          * \sa TransitionManager */
        static std::shared_ptr<LinearTransition<T>> create(T initValue, T finalValue, std::function<void(T)> updater,
                                                           float time, std::function<void()> onComplete = nullptr, const std::string& tag = "") {
            auto transition = std::shared_ptr<LinearTransition<T>>(new LinearTransition<T>{initValue, finalValue, updater, time, onComplete});
            DisplayManager::getInstance()->getTransitionManager().addTransition(tag, transition);
            return transition;
        }

        void onUpdate(float f) const override {
            m_updater((m_finalValue - m_initialValue)*f + m_initialValue);
        }

        virtual ~LinearTransition() {}
};

#endif // LINEARTRANSITION_H
