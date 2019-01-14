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

        std::string m_text = ""; ///< the text to render

    public:
        Text(GPU_Target* screen, const std::string& fontName);

        Text& operator=(const Text& t) = delete; /* Cannot copy */

        Text(const Text& t) = delete; /* Cannot copy */

        void setText(const std::string& text);

        void setX(float x);

        void setY(float y);

        virtual void render() override;

        virtual ~Text();
};

#endif // TEXT_H
