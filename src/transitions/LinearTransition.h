#ifndef LINEARTRANSITION_H
#define LINEARTRANSITION_H
#include "AbstractTransition.h"

#include <iostream>
class LinearTransition : public AbstractTransition
{
    //private:

    public:
        LinearTransition(float durationMS);

        void onUpdate(float f) const override {std::cout << f << "\n";}

        virtual ~LinearTransition();

};

#endif // LINEARTRANSITION_H
