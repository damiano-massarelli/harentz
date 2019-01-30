#ifndef BUTTON_H
#define BUTTON_H
#include "AbstractRenderable.h"
#include "Text.h"
#include "EventListenerCrumb.h"
#include "memoryUtils.h"
#include <SDL_gpu.h>
#include <memory>
#include <functional>

class Button : public AbstractRenderable, public EventListener
{
    private:
        std::unique_ptr<EventListenerCrumb> m_eventCrumb;
        std::unique_ptr<Text> m_text;

        std::function<void(Button*)> m_onClick;

        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_paddingX = 5.0f;
        float m_paddingY = 5.0f;

        imageUniquePtr m_image;

        GPU_Target* m_screen;

        std::string m_fontName;
        SDL_Color m_upColor{255, 0, 0, 255};
        SDL_Color m_downColor{0, 255, 0, 255};
        SDL_Color m_currentColor = m_upColor;

    public:
        Button(GPU_Target* screen, const std::string& font, const std::string& text = "", float paddingX = 5.0f, float paddingY = 5.0f,
               const SDL_Color& upColor = SDL_Color{241, 48, 173, 255}, const SDL_Color& downColor = SDL_Color{213, 48, 241, 255});

        virtual void render() override;

        virtual void onEvent(SDL_Event e) override;

        void setText(const std::string& text);

        void setImage(const std::string& path);

        void setOnClick(std::function<void(Button*)> onClick);

        void setX(float x);

        void setY(float y);

        float getX() const;

        float getY() const;

        float getWidth() const;

        float getHeight() const;

        virtual ~Button();
};

#endif // BUTTON_H
