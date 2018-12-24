#include "StarField.h"
#include "DisplayManager.h"
#include "randomUtils.h"
#include "colorUtils.h"
#include <cmath>

const float StarField::SPEED = 300.0f;
const SDL_Color StarField::INITIAL_COLOR{255, 0, 0, 255};
const SDL_Color StarField::FINAL_COLOR{255, 255, 0, 255};

StarField::StarField(SDL_Renderer* renderer, int numOfStars) : m_renderer{renderer}
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
    SDL_Rect starRect{0, 0, 2, 2};
    int w = DisplayManager::screenWidth();
    int h = DisplayManager::screenHeight();

    for (const auto& star : m_stars) {
        starRect.x = static_cast<int>(star->position.x);
        starRect.y = static_cast<int>(star->position.y);

        /* Determines the distance from the center of the screen on the y axis.
         * a value of 1 means the star is at the top of bottom of the screen */
        float factor = std::abs(star->position.y - h/2.0f) / (h/2.0f);
        Uint8 alpha = factor * 200; // max opacity is 200
        int starSize = static_cast<int>(5 * factor); // max size is 5
        starRect.w = starSize;
        starRect.h = starSize;

        // Linearly interpolates two colors (the equation is written that way to avoid Uint8 overflows)
        SDL_Color starColor = FINAL_COLOR * factor + (INITIAL_COLOR - INITIAL_COLOR * factor);

        // Renders the star on the screen
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, starColor.r, starColor.g, starColor.b, alpha);
        SDL_RenderFillRect(m_renderer, &starRect);

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

StarField::~StarField()
{

}

