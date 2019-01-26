#include "FntParser.h"
#include "ioUtils.h"
#include <SDL.h>

std::regex FntParser::COMMON_REG("common\\s+lineHeight=(\\d+)\\s+base=(\\d+)\\s+scaleW=(\\d+)\\s+scaleH=(\\d+)\\s+pages=(\\d+)\\s+packed=(\\d+)\\s+alphaChnl=(\\d+)\\s+redChnl=(\\d+)\\s+greenChnl=(\\d+)\\s+blueChnl=(\\d+)");
std::regex FntParser::CHAR_REG("char\\s+id=(\\d+)\\s+x=(\\d+)\\s+y=(\\d+)\\s+width=(\\d+)\\s+height=(\\d+)\\s+xoffset=(-?\\d+)\\s+yoffset=(-?\\d+)\\s+xadvance=(\\d+)\\s+page=(\\d+)\\s+chnl=(\\d+)");
std::regex FntParser::NUM_OF_CHARS_REG("chars count=(\\d+)");
std::regex FntParser::PAGE_REG("page\\s+id=(\\d+)\\s+file=\"(.*)\"");
std::regex FntParser::KERNINGS_REG("kerning\\s+first=(\\d+)\\s+second=(\\d+)\\s+amount=(-?\\d+)");

std::map<std::string, FntParser> FntParser::file2parsed; // cache

FntParser FntParser::load(const std::string& fontName)
{
    auto res = file2parsed.find(fontName);
    if (res != file2parsed.end())
        return res->second;

    FntParser parser{fontName};
    file2parsed.insert(std::make_pair(fontName, parser));

    return parser;
}

FntParser::FntParser(const std::string& fontName)
{
    std::stringstream fontConfig = readFile((fontName + ".fnt").c_str());
    if (fontConfig.bad()) {
        SDL_Log("Bad file for font %s", fontName.c_str());
        return;
    }

    parseFnt(fontConfig);

    /* Gets the average divided by two */
    m_spaceWidth /= (2 * m_numOfChars);
}

void FntParser::parseFnt(std::stringstream& fontConfig)
{
    bool commonFound = false;
    bool pageFound = false;
    int numOfCharsFound = 0;

    std::string line;
    while (std::getline(fontConfig, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, COMMON_REG)) {
            parseCommon(matches);
            commonFound = true;
        }
        else if (std::regex_search(line, matches, NUM_OF_CHARS_REG)) {
            parseNumOfChars(matches);
        }
        else if (std::regex_search(line, matches, PAGE_REG)) {
            parsePage(matches);
            pageFound = true;
        }
        else if (std::regex_search(line, matches, CHAR_REG)) {
            parseChar(matches);
            numOfCharsFound++;
        }
        else if (std::regex_search(line, matches, KERNINGS_REG)) {
            parseKerning(matches);
        }
    }

    /* Some error checking, failing silently might be a real problem */
    if (!commonFound || numOfCharsFound != m_numOfChars || !pageFound) {
        SDL_Log("Unable to correctly parse font file");
    }
}

void FntParser::parseCommon(const std::smatch& matches)
{
    m_lineHeight = std::stoi(matches.str(1));

    // sets the size of the vector based on the number of pages
    int numPages = std::stoi(matches.str(5));
    m_textureFileNames.resize(numPages);
}

void FntParser::parsePage(const std::smatch& matches)
{
    int pageId = std::stoi(matches.str(1));
    m_textureFileNames[pageId] = matches.str(2);
}

void FntParser::parseNumOfChars(const std::smatch& matches)
{
    m_numOfChars = std::stoi(matches.str(1));
}

void FntParser::parseChar(const std::smatch& matches)
{
    CharData data;
    int id = std::stoi(matches.str(1));

    data.x = std::stoi(matches.str(2));
    data.y = std::stoi(matches.str(3));
    data.w = std::stoi(matches.str(4));
    data.h = std::stoi(matches.str(5));
    data.xoff = std::stoi(matches.str(6));
    data.yoff = std::stoi(matches.str(7));
    data.xadvance = std::stoi(matches.str(8));
    data.pageIndex = std::stoi(matches.str(9));
    // channels are ignored for now

    m_spaceWidth += data.xadvance; // Will be then divided to get the average

    m_char2data.insert(std::make_pair(id, data));
}

void FntParser::parseKerning(const std::smatch& matches)
{
    int prev = std::stoi(matches.str(1));
    int current = std::stoi(matches.str(2));

    int offset = std::stoi(matches.str(3));
    m_kernings.insert(std::make_pair(std::make_pair(prev, current), offset));
}

const std::vector<std::string>& FntParser::getTextureFileNames() const
{
    return m_textureFileNames;
}

const CharData* FntParser::getDataForChar(int charId) const
{
    const auto& iter = m_char2data.find(charId);
    if (iter != m_char2data.end())
        return &(iter->second);

    return nullptr; // Not found :(
}

const int FntParser::getKerningSpace(int prevChar, int currentChar) const
{
    const auto& iter = m_kernings.find(std::make_pair(prevChar, currentChar));
    if (iter == m_kernings.end())
        return 0; // not found

    return iter->second;
}

const float FntParser::getOffsetForSpace() const
{
    return m_spaceWidth;
}

const int FntParser::getLineHeight() const
{
    return m_lineHeight;
}


FntParser::~FntParser()
{
    //dtor
}
