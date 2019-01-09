#ifndef STARFIELD_H
#define STARFIELD_H
#include "Point3.h"
#include <SDL.h>
#include <SDL_gpu.h>
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
        static const SDL_Color FINAL_COLOR_BONUS;

        GPU_Target* m_screen;
        std::vector<std::unique_ptr<Star>> m_stars;

        float m_updatePositionSpeed = 0; ///< the speed to use to update the position of the stars. Based on elapsedMs (see update)
        SDL_Color m_finalColor = FINAL_COLOR; ///< the final color of stars can change when a bonus is activated.

    public:
        StarField(GPU_Target* screen, int numOfStars = 200);

        virtual void render() override;

        void update(float elapsedMs);

        void enableBonusFinalColor();

        virtual ~StarField();
};

#endif // STARFIELD_H
