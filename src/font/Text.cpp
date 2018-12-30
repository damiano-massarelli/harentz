#include "Text.h"
#include "ioUtils.h"

Text::Text(GPU_Target* screen, const std::string& fontName) : m_screen{screen}, m_fntParser{fontName}
{
    // Loads the texture
    m_glyphTexture = GPU_LoadImage(("resources/font/" + m_fntParser.getTextureFileName()).c_str());
}

void Text::setText(const std::string& text)
{
    m_text = text;
}

void Text::render()
{
    float xcur = m_x; /* The horizontal cursor */
    float ycur = m_y; /* The vertical cursor */
    for (char c : m_text) {
        if (c == ' ') {
            xcur += m_fntParser.getOffsetForSpace();
            continue;
        }
        const CharData* d = m_fntParser.getDataForChar(static_cast<int>(c));
        if (d == nullptr) continue; // This is an invalid character (maybe we should use something known)
        GPU_Rect glyphRect{static_cast<float>(d->x), static_cast<float>(d->y),
                           static_cast<float>(d->w), static_cast<float>(d->h)};

        /* !! GPU_Blit (as opposed to GPU_BlitRect) uses the x and y coords for the center of the image add w/2 and h/2) */
        GPU_Blit(m_glyphTexture, &glyphRect, m_screen, xcur + d->xoff + d->w/2, ycur + d->h/2 + d->yoff);

        xcur += d->xadvance;
    }
}

void Text::setX(float x)
{
    m_x = x;
}

void Text::setY(float y)
{
    m_y = y;
}

Text::~Text()
{
    GPU_FreeImage(m_glyphTexture);
}
