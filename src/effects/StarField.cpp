#include "StarField.h"
#include "DisplayManager.h"
#include "randomUtils.h"
#include "colorUtils.h"
#include "LinearTransition.h"
#include <cmath>

const float StarField::SPEED = 300.0f;
const SDL_Color StarField::INITIAL_COLOR{0, 255, 249, 255};
const SDL_Color StarField::FINAL_COLOR{150, 0, 255, 255};
const SDL_Color StarField::FINAL_COLOR_BONUS{255, 0, 193, 255};

StarField::StarField(GPU_Target* screen, int numOfStars) : m_screen{screen}
{
    int w = DisplayManager::screenWidth();
    int h = DisplayManager::screenHeight();

    for (int i = 0; i < numOfStars; ++i) {
        Point3 pos{w/2.0f, h/2.0f, 0, 1}; // Centers the star
        Point3 dir{randRange(-1.0f, 1.0f), randRange(-1.0f, 1.0f), 0, 0}; // Assigns a random direction
        m_stars.push_back(std::make_unique<Star>(pos, dir));
    }
}

void StarField::render()
{
    GPU_Rect starRect{0, 0, 2, 2};
    int w = DisplayManager::screenWidth();
    int h = DisplayManager::screenHeight();

    for (const auto& star : m_stars) {
        starRect.x = static_cast<int>(star->position.x);
        starRect.y = static_cast<int>(star->position.y);

        /* Determines the distance from the center of the screen on the y axis.
         * a value of 1 means the star is at the top of bottom of the screen */
        float factor = std::abs(star->position.y - h/2.0f) / (h/2.0f);

        int starSize = static_cast<int>(3 * factor) + 1; // max size is 4, min size is 1
        starRect.w = starSize;
        starRect.h = starSize;

        // Linearly interpolates two colors (the equation is written that way to avoid Uint8 overflows)
        SDL_Color starColor = m_finalColor * factor + INITIAL_COLOR * (1.0f - factor);

        // Renders the star on the screen
        GPU_RectangleFilled2(m_screen, starRect, starColor);

        /* Updates the position of the star. Takes it back to the origin if it is outside the screen */
        star->position = star->position + (star->direction * m_updatePositionSpeed);
        if (star->position.x < 0 || star->position.x > h || star->position.y < 0 || star->position.y > h)
            star->position = Point3{w/2.0f, h/2.0f, 0};

    }
}

void StarField::update(float elapsedMs)
{
    // Sets the speed that will be used to update the starts
    m_updatePositionSpeed = SPEED * (elapsedMs / 1000.0f);
}

void StarField::enableBonusFinalColor()
{
    m_finalColor = FINAL_COLOR_BONUS;

    /* Uses a transition to perform an action after 1.5 sec */
    LinearTransition<int>::create(0, 0, nullptr, 1500.0f, [this]{this->m_finalColor = FINAL_COLOR;}, "game");
}

StarField::~StarField()
{

}

