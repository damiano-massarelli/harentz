#include "BreakingParticles.h"
#include "LinearTransition.h"
#include "DisplayManager.h"
#include <cmath>
#include "Shape.h"
#include "Mat4.h"

const float BreakingParticles::CUBE_SIDE_SIZE = 10.0f;
std::shared_ptr<Shape> BreakingParticles::CUBE_SHAPE = std::shared_ptr<Shape>(
                     new Shape{std::vector<Point3>{
                     Point3{-BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{-BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{-BreakingParticles::CUBE_SIDE_SIZE, -BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE},
                     Point3{-BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE, BreakingParticles::CUBE_SIDE_SIZE}},
                  std::vector<int>{
                         1, 5, 7, 3, // up face
                         1, 4, 5, 2, // right face
                         0, 3, 7, 6, // left face
                         0, 1, 2, 3, // front face
                         0, 6, 4, 1}}); // top face

float randRange(float start, float end) {
    float f = static_cast<float>(std::rand())/(RAND_MAX + 1u);
    return start + (end - start)*f;
}

AbstractRenderable* BreakingParticles::create(Renderer* renderer, const Point3& position, const SDL_Color& color, int numOfPieces)
{
    BreakingParticles* particleEffect = new BreakingParticles{renderer, position, color, numOfPieces};

    // TODO: should use a unique_ptr but there are some problems when the lambda is copied.
    /* This linear transition will move the particles, see the update method */
    LinearTransition<float>::create(0.0f, 1.0f,
                             [particle = std::shared_ptr<BreakingParticles>(particleEffect)](float f) {
                                particle->update(f);
                             },
                             500.0f,
                             nullptr,
                             "game");

    return particleEffect;
}

BreakingParticles::BreakingParticles(Renderer* renderer, const Point3& position, const SDL_Color& color, int numOfPieces) : m_renderer{renderer}
{
    /** builds a circle whose radius is the min between screenWidth and screenHeight. The destination
      * of each particle is on this circle */
    int destinationRadius = std::min(DisplayManager::screenWidth(), DisplayManager::screenHeight());
    for (int i = 0; i < numOfPieces; ++i) {
        float angle = randRange(0.0f, 2 * M_PI);
        float finalX = position.x + destinationRadius * std::cos(angle);
        float finalY = position.y + destinationRadius * std::sin(angle);
        float finalZ = position.z - randRange(100.0f, 550.0f);

        std::unique_ptr<Transform> cubeParticle = std::make_unique<Transform>(CUBE_SHAPE);
        m_directions.push_back(std::make_pair(position, Point3{finalX, finalY, finalZ})); // Direction of each particle represented by start and end point

        cubeParticle->setColor(color); // color of the broken piece
        float randomRotation = randRange(-M_PI/2.0f, M_PI/2.0f);
        cubeParticle->setTransformationMatrix(rotation(randomRotation, randomRotation, randomRotation));
        m_particles.push_back(std::move(cubeParticle));
    }
}

void BreakingParticles::update(float f)
{
    // updates the position and color of each particle (f is in the range [0, 1])
    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        const Point3& src = m_directions[i].first;
        const Point3& dst = m_directions[i].second;
        m_particles[i]->setPosition((dst - src)*f + src);

        // updates color, when the animation is over alpha = 0
        SDL_Color current = m_particles[i]->getColor();
        current.a = 255 * (1.0f - f);
        m_particles[i]->setColor(current);
    }
}

void BreakingParticles::render()
{
    for (const auto& particle : m_particles)
        m_renderer->render(*particle);
}

BreakingParticles::~BreakingParticles()
{

}
