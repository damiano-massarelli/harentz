#include "Ground.h"
#include <vector>
#include "Mat4.h"

#include <cmath>

/*
 * Creates a shape that represents the lanes of the ground. The indices
 * of the lanes are here reported:
 *
 *      i       i+2
 *       |     |
 *       |     |
 *       |     |
 *    i+1       i+3 */
Ground::Ground(Renderer* renderer, int screenWidth, int screenHeight, int numOfLanes) : Transform{nullptr}, m_screenWidth{screenWidth}, m_screenHeight{screenHeight}
{
    setRenderer(renderer);

    /* Builds a shape representing the lanes */
    float laneWidth = static_cast<float>(screenWidth) / numOfLanes;
    std::vector<Point3> vertices;
    for (int i = 0; i <= numOfLanes; i++) { // <= a single lane is composed of 4 vertices
        vertices.push_back(Point3{i * laneWidth, 0.0f, Ground::GROUND_DEPTH});
        vertices.push_back(Point3{i * laneWidth, 0.0f, 0.0f});
    }

    std::vector<int> indices;
    for (int i = 0; i < numOfLanes * 2; i+=2)
        indices.insert(indices.end(), {i, i+2, i+3, i+1});

    setShape(std::make_shared<Shape>(vertices, indices));

    setFillColor(SDL_Color{0, 0, 0, 0}); // Please don't draw as a solid object (alpha = 0)

    /* Calculate the rotation of the ground */
    computeRotation();
    setTransformationMatrix(m_rotationMatrix);

    setPosition(Point3{-screenWidth/2.0f, screenHeight/2.0f, 0.0f});
}

void Ground::computeRotation()
{
    // Computes the y position of a point with z = GROUND_DEPTH that is projected in the 2d point (0, -screenHeight/4)
    float depthY = getRenderer()->inverseProjection(SDL_Point{0, -m_screenHeight/4}, Ground::GROUND_DEPTH).y;

    // computes the rotation matrix so that once the ground is translated down (-screenHeight/2)
    // it points towards the point whose y is depthY
    float angle = asin((depthY - m_screenHeight/2) / Ground::GROUND_DEPTH);
    m_rotationMatrix = rotation(-angle, 0.0f, 0.0f);
}

const Mat4& Ground::getRotationMatrix() const
{
    return m_rotationMatrix;
}

Point3 Ground::getSpawnPoint() const
{
    return Point3{0.0f, getVertWorldPositions()[0].y, getVertWorldPositions()[0].z};
}

Ground::~Ground()
{
    //dtor
}
