#ifndef FNTPARSER_H
#define FNTPARSER_H
#include <regex>
#include <sstream>
#include <string>
#include <map>
#include <vector>

struct CharData {
    int x; ///< x in the texture
    int y; ///< y in the texture
    int w; ///< width in the texture
    int h; ///< height in the texture
    int xoff; ///< xoffset for cursor
    int yoff; ///< y offset for the cursor
    int xadvance; ///< how much the cursor should advance
    int pageIndex; ///< the glyph page on which this char is stored
};

/** \brief a parser for .fnt files
  * .fnt files can be generated using http://www.angelcode.com/products/bmfont/documentation.html */
class FntParser
{
    private:
        // TODO: this cache is never emptied. This is not a problem if the app uses a limited amount of fonts
        static std::map<std::string, FntParser> file2parsed; ///< a cache used to maintain already parsed fonts

        /* Regex for matching fnt lines */
        static std::regex COMMON_REG; ///< matches the "common" part
        static std::regex CHAR_REG; ///< matches the "char" part
        static std::regex NUM_OF_CHARS_REG; ///< matches the "chars count" part
        static std::regex PAGE_REG; ///< matches the "page" part
        static std::regex KERNINGS_REG; ///< matches for "kernings" page

        std::map<int, CharData> m_char2data; ///< data for each char
        std::map<std::pair<int, int>, int> m_kernings; ///< maps a pair of char codes to the amount the second should be moved on the x axis
        std::vector<std::string> m_textureFileNames;
        int m_lineHeight = 0; ///< the height of each line
        float m_spaceWidth = 0.0f; ///< the width of a space (to increase the cursor)
        int m_numOfChars = 0; ///< the number of chars used by this font

        /** \brief parses the .fnt file and set the status of this text object */
        void parseFnt(std::stringstream& fontConfig);

        /** \brief parses the "common" part of a .fnt file */
        void parseCommon(const std::smatch& matches);

        /** \brief parses the "chars count" part of the file */
        void parseNumOfChars(const std::smatch& matches);

        /** \brief parses the "char" part of a .fnt file */
        void parseChar(const std::smatch& matches);

        /** \brief parses the "page" part of a .fnt file */
        void parsePage(const std::smatch& matches);

        /** \brief parses the "kerning" part of a .fnt file */
        void parseKerning(const std::smatch& matches);

    public:
        /** \brief loads a parser and puts it in the cache
          * it is similar to the constructor but it also adds the font to a cache.
          * The cache is only available when this method is used */
        static FntParser load(const std::string& fontName);

        FntParser(const std::string& fontName);

        /** \brief returns the names of the files containing the glyph texture */
        const std::vector<std::string>& getTextureFileNames() const;

        /** \brief returns the data used to render a character.
          * \param charId the id (ascii code) of the character
          * \return a pointer to a CharData structure, nullptr if that character is not
          *         available
          * \sa CharData */
        const CharData* getDataForChar(int charId) const;

        /** \brief returns the kerning distance between two characters
          * \param prevChar the character preceding the current one
          * \param currentChar the character that is being rendered
          * \return the kerning distance for these two characters. 0 if it is not defined */
        const int getKerningSpace(int prevChar, int currentChar) const;

        /** \brief returns the x offset for the space character */
        const float getOffsetForSpace() const;

        /** \brief returns the height of a line */
        const int getLineHeight() const;

        virtual ~FntParser();

};

#endif // FNTPARSER_H
