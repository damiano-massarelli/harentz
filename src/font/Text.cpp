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

    int previousCharCode = -1;
    for (char c : m_text) {
        int charCode = static_cast<int>(c); // the code of this char
        if (c == ' ') {
            xcur += m_fntParser.getOffsetForSpace();
            previousCharCode = charCode;
            continue;
        }
        else if (c == '\n') {
            xcur = m_x;
            ycur += m_fntParser.getLineHeight();
            previousCharCode = charCode;
        }

        const CharData* d = m_fntParser.getDataForChar(charCode);
        int xoffKerning = m_fntParser.getKerningSpace(previousCharCode, charCode);
        previousCharCode = charCode;

        if (d == nullptr) continue; // This is an invalid character (maybe we should use something known)
        GPU_Rect glyphRect{static_cast<float>(d->x), static_cast<float>(d->y),
                           static_cast<float>(d->w), static_cast<float>(d->h)};

        /* !! GPU_Blit (as opposed to GPU_BlitRect) uses the x and y coords for the center of the image add w/2 and h/2) */
        GPU_Blit(m_glyphTexture, &glyphRect, m_screen, xcur + d->xoff + d->w/2 + xoffKerning, ycur + d->h/2 + d->yoff);

        xcur += d->xadvance + xoffKerning;
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
