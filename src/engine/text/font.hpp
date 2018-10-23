#pragma once

#include <GL/glew.h>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "../../math/vec.hpp"

struct Character {
    // The character represented by this Character
    char character;
    // ID handle of the glyph texture
    GLuint textureID;
    // Size of glyph (width, height)
    Vec2<int> size;
    // Offset from baseline to left/top of glyph (left, top)
    Vec2<int> bearing;
    // Offset to advance to next glyph
    GLuint advance;
};

class Font {
public:
    // Initializes a new font with the specified size.
    Font(const std::string& fontPath, int size);
    virtual ~Font();

    Character generateCharacter(const char character);
private:
    // Used to cache characters
    std::map<char, Character> m_characters;

    // Some characters have different heights and this causes problems. This fixes that.
    int m_textHeight;

    // Used to generate characters
    FT_Library m_fontLibrary;
    FT_Face m_fontFace;
};