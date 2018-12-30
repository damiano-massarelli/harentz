#ifndef FNTPARSER_H
#define FNTPARSER_H
#include <regex>
#include <sstream>
#include <string>
#include <map>

struct CharData {
    int x; ///< x in the texture
    int y; ///< y in the texture
    int w; ///< width in the texture
    int h; ///< height in the texture
    int xoff; ///< xoffset for cursor
    int yoff; ///< y offset for the cursor
    int xadvance; ///< how much the cursor should advance
};

/** \brief a parser for .fnt files */
class FntParser
{
    private:
        /* Regex for matching fnt lines */
        static std::regex COMMON_REG; ///< matches the "common" part
        static std::regex CHAR_REG; ///< matches the "char" part
        static std::regex NUM_OF_CHARS_REG; ///< matches the "chars count" part
        static std::regex PAGE_REG; ///< matches the "page" part

        std::map<int, CharData> m_char2data; ///< data for each char
        std::string m_textureFileName;
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

    public:
        FntParser(const std::string& fontName);

        /** \brief returns the name of the file containing the glyph texture */
        const std::string& getTextureFileName() const;

        /** \brief returns the data used to render a character.
          * \param charId the id (ascii code) of the character
          * \return a pointer to a CharData structure, nullptr if that character is not
          *         available
          * \sa CharData */
        const CharData* getDataForChar(int charId) const;

        /** \brief returns the x offset for the space character */
        const float getOffsetForSpace() const;

        virtual ~FntParser();

};

#endif // FNTPARSER_H
