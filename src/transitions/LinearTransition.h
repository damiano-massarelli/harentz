#ifndef LINEARTRANSITION_H
#define LINEARTRANSITION_H
#include "AbstractTransition.h"
#include "TransitionManager.h"
#include "DisplayManager.h"
#include <memory>
#include <functional>

template <class Tc>
class LinearTransition : public AbstractTransition
{
    private:
        std::function<void(Tc)> m_updater;
        Tc m_initialValue{0.0f};
        Tc m_finalValue{0.0f};

    public:
        LinearTransition(Tc initialValue, Tc finalValue, std::function<void(Tc)> updater, float durationMS) :
            AbstractTransition{durationMS}, m_initialValue{initialValue}, m_finalValue{finalValue}, m_updater{updater}  {}

        void onUpdate(float f) const override {
            m_updater((m_finalValue - m_initialValue)*f + m_initialValue);
        }

        virtual ~LinearTransition() {}
};

template <typename T>
std::shared_ptr<LinearTransition<T>> createLT(T initValue, T finalValue, std::function<void(T)> updater, float time) {
    auto transition = std::shared_ptr<LinearTransition<T>>(new LinearTransition<T>{initValue, finalValue, updater, time});
    DisplayManager::getInstance()->getTransitionManager().addTransition("game", transition);
    return transition;
}

#endif // LINEARTRANSITION_H
