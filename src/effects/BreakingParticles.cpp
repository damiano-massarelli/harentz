#include "BreakingParticles.h"
#include "LinearTransition.h"
#include "DisplayManager.h"
#include <cmath>
#include "Shape.h"

float randRange(float start, float end) {
    float f = static_cast<float>(std::rand())/(RAND_MAX + 1u);
    return start + (end - start)*f;
}
float cubeSize = 20.0f;
std::shared_ptr<Shape> CUBE_SHAPE = std::shared_ptr<Shape>(
                     new Shape{std::vector<Point3>{ Point3{-cubeSize, -cubeSize, -cubeSize},
                     Point3{cubeSize, -cubeSize, -cubeSize},
                     Point3{cubeSize, cubeSize, -cubeSize},
                     Point3{-cubeSize, cubeSize, -cubeSize},
                     Point3{cubeSize, -cubeSize, cubeSize},
                     Point3{cubeSize, cubeSize, cubeSize},
                     Point3{-cubeSize, -cubeSize, cubeSize},
                     Point3{-cubeSize, cubeSize, cubeSize}},
                  std::vector<int>{
                         1, 4, 5, 2, // right face
                         0, 3, 7, 6, // left face
                         0, 1, 2, 3, // front face
                         0, 6, 4, 1}}); // top face

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
        float angle = randRange(0.0f, 2 * 3.141592f);
        float finalX = position.x + destinationRadius * std::cos(angle);
        float finalY = position.y + destinationRadius * std::sin(angle);
        float finalZ = position.z - randRange(0.0f, 550.0f);
        std::unique_ptr<Transform> cubeParticle = std::make_unique<Transform>(CUBE_SHAPE);
        cubeParticle->setColor(color);
        m_particles.push_back(std::move(cubeParticle));
        m_directions.push_back(std::make_pair(position, Point3{finalX, finalY, finalZ}));
    }
}

void BreakingParticles::update(float f)
{
    // updates the position of each particle (f is in the range [0, 1])
    for (int i = 0; i < m_particles.size(); ++i) {
        const Point3& src = m_directions[i].first;
        const Point3& dst = m_directions[i].second;
        m_particles[i]->setPosition((dst - src)*f + src);
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
