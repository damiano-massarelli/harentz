#ifndef STARFIELD_H
#define STARFIELD_H
#include "Point3.h"
#include "SDL2/SDL.h"
#include "AbstractRenderable.h"
#include <memory>
#include <vector>

struct Star {
    Star(const Point3& pos, const Point3& dir) : position{pos}, direction{dir} {}
    Point3 position; // it is actually a 2D point
    Point3 direction; // it is actually a 2D direction
};

class StarField : public AbstractRenderable
{
    private:
        static const float SPEED; ///< pixels per second
        static const SDL_Color INITIAL_COLOR;
        static const SDL_Color FINAL_COLOR;

        SDL_Renderer* m_renderer;
        std::vector<std::unique_ptr<Star>> m_stars;

        float m_updatePositionSpeed = 0; ///< the speed to use to update the position of the stars. Based on elapsedMs (see update)

    public:
        StarField(SDL_Renderer* renderer, int numOfStars = 200);

        virtual void render() override;

        void update(float elapsedMs);

        virtual ~StarField();
};

#endif // STARFIELD_H
