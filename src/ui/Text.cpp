#include "Text.h"
#include "ioUtils.h"
#include <sstream>

Text::Text(GPU_Target* screen, const std::string& fontName) : m_screen{screen}, m_fntParser{FntParser::load(fontName)}
{
    const std::vector<std::string>& textureFilenames = m_fntParser.getTextureFileNames();
    m_glyphTextures.resize(textureFilenames.size());

    // Loads the texture
    for (std::size_t i = 0; i < textureFilenames.size(); ++i)
        m_glyphTextures[i] = GPU_LoadImage(("resources/font/" + textureFilenames[i]).c_str());
}

void Text::setText(const std::string& text)
{
    m_text = text;
    goThroughText(); // computes the width and height
}

void Text::goThroughText(std::function<void(const CharData*, GPU_Rect*, float, float)> func)
{
    float xcur = m_x; /* The horizontal cursor */
    float ycur = m_y; /* The vertical cursor */

    int previousCharCode = -1;
    for (int i = 0; i < m_text.length(); ++i) {
        char c = m_text[i];
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
        /* Set text color on the fly. \033 followed by 32 hex bits representing a color */
        else if (c == '\033') {
            Uint8 r;
            Uint8 g;
            Uint8 b;
            Uint8 a;
            Uint32 color;
            std::stringstream ss;
            ss << std::hex << m_text.substr(i+1, 8);
            ss >> color;
            r = (color & 0xFF000000) >> 24;
            g = (color & 0x00FF0000) >> 16;
            b = (color & 0x0000FF00) >> 8;
            a = (color & 0X000000FF);
            setColor(SDL_Color{r, g, b, a});
            i += 8; // skip color declaration
            continue;
        }

        const CharData* d = m_fntParser.getDataForChar(charCode);
        int xoffKerning = m_fntParser.getKerningSpace(previousCharCode, charCode);
        previousCharCode = charCode;

        if (d == nullptr) continue; // This is an invalid character (maybe we should use something known)
        GPU_Rect glyphRect{static_cast<float>(d->x), static_cast<float>(d->y),
                           static_cast<float>(d->w), static_cast<float>(d->h)};

        if (func) /* !! GPU_Blit (as opposed to GPU_BlitRect) uses the x and y coords for the center of the image add w/2 and h/2) */
            func(d, &glyphRect, xcur + d->xoff + d->w/2 + xoffKerning, ycur + d->h/2 + d->yoff);

        xcur += d->xadvance + xoffKerning;
    }

    m_width = xcur - m_x;
    m_height = ycur - m_y + m_fntParser.getLineHeight(); // takes into account at least a line
}

void Text::render()
{
    goThroughText([this](const CharData* d, GPU_Rect* glyphRect, float x, float y) {
                    GPU_Blit(this->m_glyphTextures[d->pageIndex], glyphRect, this->m_screen, x, y);
                  });
}

void Text::setX(float x)
{
    m_x = x;
}

void Text::setY(float y)
{
    m_y = y;
}

void Text::setColor(const SDL_Color& color)
{
    for (auto glyphTexture : m_glyphTextures)
        GPU_SetColor(glyphTexture, color);
}

float Text::getX() const
{
    return m_x;
}

float Text::getY() const
{
    return m_y;
}

float Text::getWidth() const
{
    return m_width;
}

float Text::getHeight() const
{
    return m_height;
}

const FntParser& Text::getFntParser()
{
    return m_fntParser;
}

Text::~Text()
{
    for (const auto texture : m_glyphTextures)
        GPU_FreeImage(texture);
}
