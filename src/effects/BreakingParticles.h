#ifndef BREAKINGPARTICLES_H
#define BREAKINGPARTICLES_H
#include "Renderer.h"
#include "AbstractRenderable.h"
#include <vector>
#include <memory>

class BreakingParticles : AbstractRenderable
{
    private:
        std::vector<std::unique_ptr<Transform>> m_particles; ///< particles to display
        std::vector<std::pair<Point3, Point3>> m_directions; ///< the direction of the corresponding (same index) particle (represented as initial and final positions)
        Renderer* m_renderer;

        /** \brief Constructor. Private so that a smart pointer can be created
          * \sa create */
        BreakingParticles(Renderer* renderer, const Point3& position, const SDL_Color& color, int numOfPieces);

    public:
        /** \brief Creates a new particle effect and returns its renderable "part"
          * \param renderer the 3d renderer used for the particles
          * \param position the origin point for the particles
          * \param color the color of the particles
          * \param numOfPieces the number of particle to create
          * \return the renderable part of the particles
          * Memory is automatically managed for all the particles. Moreover, they will be automatically
          * removed by the scene once all the particles are dead. (just like any other \sa AbstractRenderable) */
        static AbstractRenderable* create(Renderer* renderer, const Point3& position, const SDL_Color& color, int numOfPieces);

        /** \brief updates the position of all the particles
          * \param the progress percentage must be in [0, 1] */
        void update(float f);

        virtual void render() override;

        virtual ~BreakingParticles();

};

#endif // BREAKINGPARTICLES_H
