#include "StarField.h"
#include "DisplayManager.h"
#include "randomUtils.h"
#include <cmath>

const float StarField::SPEED = 300.0f;

StarField::StarField(SDL_Renderer* renderer, int numOfStars) : m_renderer{renderer}
{
    int w = DisplayManager::screenWidth();
    int h = DisplayManager::screenHeight();
    for (int i = 0; i < numOfStars; ++i) {
        Point3 pos{w/2.0f, h/2.0f, 0, 1};
        Point3 dir{randRange(-1.0f, 1.0f), randRange(-1.0f, 1.0f), 0, 0};
        m_stars.push_back(std::make_unique<Star>(pos, dir));
    }
}

void StarField::render()
{
    SDL_Rect starRect{0, 0, 2, 2};
    for (const auto& star : m_stars) {
        starRect.x = static_cast<int>(star->position.x);
        starRect.y = static_cast<int>(star->position.y);
        float factor = std::abs(star->position.y - DisplayManager::screenHeight()/2.0f) / DisplayManager::screenHeight();
        Uint8 alpha = factor * 255;
        int starSize = static_cast<int>(10 * factor);
        starRect.w = starSize;
        starRect.h = starSize;
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, alpha);
        SDL_RenderFillRect(m_renderer, &starRect);
    }
}

void StarField::update(float elapsedMs)
{
    float speed = SPEED * (elapsedMs / 1000.0f);
    for (const auto& star : m_stars) {
        star->position = star->position + star->direction * speed;
        if (star->position.x < 0 || star->position.x > DisplayManager::screenWidth() || star->position.y < 0 || star->position.y > DisplayManager::screenHeight())
            star->position = Point3{DisplayManager::screenWidth()/2.0f, DisplayManager::screenHeight()/2.0f, 0};
    }
}

StarField::~StarField()
{

}

