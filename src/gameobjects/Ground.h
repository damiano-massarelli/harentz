#ifndef GROUND_H
#define GROUND_H
#include "Transform.h"
#include "Renderer.h"
#include "Mat4.h"
#include "Point3.h"
#include <memory>

class Ground : public Transform
{
    private:
        int m_screenWidth = -1;
        int m_screenHeight = -1;
        Mat4 m_rotationMatrix;

        void computeRotation();

    public:
        static constexpr float GROUND_DEPTH = 3500.0f;

        Ground(Renderer* renderer, int screenWidth, int screenHeight, int numOfLanes = 5);

        /** \brief returns the rotation matrix for the ground
          * the rotation matrix represent the transformation used
          * to tilt the ground */
        const Mat4& getRotationMatrix() const;

        /** \brief returns the spawn point for the pieces
          * the spawn point is the deepest point of the ground mesh */
        Point3 getSpawnPoint() const;

        virtual ~Ground();
};

#endif // GROUND_H
