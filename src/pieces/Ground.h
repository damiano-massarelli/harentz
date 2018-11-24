#ifndef GROUND_H
#define GROUND_H
#include "Transform.h"
#include "Shape.h"
#include "Renderer.h"
#include <memory>

class Ground : public Transform
{
    public:
        static constexpr float GROUND_DEPTH = 1500.0f;

        Ground(Renderer* renderer, int screenWidth, int screenHeight, int numOfLanes = 5);

        void resize();

        virtual ~Ground();
};

#endif // GROUND_H
