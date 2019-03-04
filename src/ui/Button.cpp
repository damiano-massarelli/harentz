#include "Button.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include <algorithm>

Button::Button(GPU_Target* screen, const std::string& font, const std::string& text, float paddingX, float paddingY,
               const SDL_Color& upColor, const SDL_Color& downColor, const SDL_Color& textColor) : m_screen{screen}, m_fontName{font},
               m_paddingX{paddingX}, m_paddingY{paddingY}, m_upColor{upColor}, m_downColor{downColor}
{
    m_text = std::make_unique<Text>(screen, font);
    m_text->setText(text);
    m_text->setColor(textColor);

    // listeners for finger  up down and mouse up down
    EventManager& evtManager = DisplayManager::getInstance()->getEventManager();
    m_eventCrumb = evtManager.addListenerFor(SDL_FINGERDOWN, this, true);
    evtManager.addListenerFor(SDL_FINGERUP, m_eventCrumb.get());
    evtManager.addListenerFor(SDL_MOUSEBUTTONDOWN, m_eventCrumb.get());
    evtManager.addListenerFor(SDL_MOUSEBUTTONUP, m_eventCrumb.get());
}

void Button::render()
{
    GPU_RectangleRoundFilled(m_screen, m_x, m_y, m_x + getWidth(), m_y + getHeight(), getHeight()/5, m_currentColor);
    GPU_Blit(m_image.get(), nullptr, m_screen, m_x + getWidth()/2, m_y + getHeight()/2);
    m_text->render();
}

void Button::setText(const std::string& text)
{
    m_text->setText(text);
}

void Button::setImage(const std::string& path)
{
    m_image = imageUniquePtr{GPU_LoadImage(path.c_str())};
    m_image->anchor_x = 0.5f;
    m_image->anchor_y = 0.5f;
}

void Button::setOnClick(std::function<void(Button*)> onClick)
{
    m_onClick = onClick;
}

void Button::onEvent(SDL_Event e)
{
    float x, y;
    bool down = e.type == SDL_FINGERDOWN || e.type == SDL_MOUSEBUTTONDOWN;
    if (e.type == SDL_FINGERDOWN || e.type == SDL_FINGERUP) {
        x = e.tfinger.x * DisplayManager::screenWidth();
        y = e.tfinger.y * DisplayManager::screenHeight();
    } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        x = e.button.x;
        y = e.button.y;
    }
    if (x > m_x && x < m_x + getWidth() && y > m_y && y < m_y + getHeight()) {
        if (down)
            m_currentColor = m_downColor;
        else {
            m_currentColor = m_upColor;
            if (m_onClick)
                m_onClick(this);
        }
    } else
        m_currentColor = m_upColor;
}

void Button::setX(float x)
{
    m_x = x;
    m_text->setX(x + m_paddingX/2);
}

void Button::setY(float y)
{
    m_y = y;
    m_text->setY(y + m_paddingY/2);
}

float Button::getX() const
{
    return m_x;
}

float Button::getY() const
{
    return m_y;
}

float Button::getWidth() const
{
    float width = m_text->getWidth();
    if (m_image)
        width = std::max(static_cast<float>(m_image->w), width);
    return width + m_paddingX;
}

float Button::getHeight() const
{
    float height = m_text->getHeight();
    if (m_image)
        height = std::max(static_cast<float>(m_image->h), height);
    return height + m_paddingY;
}

Button::~Button()
{

}
