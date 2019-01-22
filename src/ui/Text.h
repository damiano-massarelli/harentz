#ifndef TEXT_H
#define TEXT_H
#include "AbstractRenderable.h"
#include "FntParser.h"
#include <SDL_gpu.h>
#include <map>
#include <string>
#include <regex>
#include <sstream>

class Text : public AbstractRenderable
{
    private:
        GPU_Target* m_screen = nullptr; ///< where text is going to be rendered
        std::vector<GPU_Image*> m_glyphTextures; ///< the textures containing all the glyphs

        FntParser m_fntParser; ///< contains data related to the glyphs

        float m_x = 0.0f; ///< x position
        float m_y = 0.0f; ///< y position
        float m_width = 0.0f; ///< with of this text
        float m_height = 0.0f; ///< height of this text

        std::string m_text = ""; ///< the text to render

    public:
        Text(GPU_Target* screen, const std::string& fontName);

        Text& operator=(const Text& t) = delete; /* Cannot copy */

        Text(const Text& t) = delete; /* Cannot copy */

        const FntParser& getFntParser();

        void setText(const std::string& text);

        void setX(float x);

        void setY(float y);

        void setColor(const SDL_Color& color);

        float getX() const;

        float getY() const;

        float getWidth() const;

        float getHeight() const;

        /** \brief Goes through all the characters of the text calling a function for each char
          * This is used to calculate the size of the text and to render it. The first parameter
          * contains all the data used to render the glyph. The second parameter contains the rect that
          * contains the glyph in the texture and third and fourth parameter represent
          * the coordinates for that glyph */
        void goThroughText(std::function<void(const CharData*, GPU_Rect*, float, float)> func = nullptr);

        /** \brief renders the text on an image
          * \return an image containing the text, the caller is responsible for freeing the image
          * this is useful if transformation are to be applied to the rendered text
          *
          * not supported now due to a `bug` in sdl_gpu */
        GPU_Image* renderToImage();

        virtual void render() override;

        virtual ~Text();
};

#endif // TEXT_H
