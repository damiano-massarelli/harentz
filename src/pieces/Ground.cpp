#include "Ground.h"
#include <vector>
#include "Mat4.h"
/*
 * Creates a shape that represents the lanes of the ground. The indices
 * of the lanes are here reported:
 *
 *      i       i+2
 *       |     |
 *       |     |
 *       |     |
 *    i+1       i+3 */
Ground::Ground(Renderer* renderer, int screenWidth, int screenHeight, int numOfLanes) : Transform{nullptr}
{
    setRenderer(renderer);

    float laneWidth = static_cast<float>(screenWidth) / numOfLanes;
    std::vector<Point3> vertices;
    for (int i = 0; i <= numOfLanes; i++) { // <= a single lane is composed of 4 vertices
        vertices.push_back(Point3{i * laneWidth, 0.0f, Ground::GROUND_DEPTH});
        vertices.push_back(Point3{i * laneWidth, 0.0f, 0.0f});
    }

    std::vector<int> indices;
    for (int i = 0; i < numOfLanes * 2; i+=2) {
        indices.insert(indices.end(), {i, i+2, i+3, i+1});
    }

    setShape(std::make_shared<Shape>(vertices, indices));

    // Centers the ground on the x-axis
    setPosition(Point3{-screenWidth/2.0f, screenHeight/2.0f, 0.0f});
    setColor(SDL_Color{0, 0, 0, 255});
}

void Ground::resize()
{
    float currentZ = getVertWorldPositions()[0].z;
    float scaleFactor = Ground::GROUND_DEPTH / currentZ;
    setTransformationMatrix(getTransformationMatrix() * scale(1.0f, 1.0f, scaleFactor));
}


Ground::~Ground()
{
    //dtor
}
