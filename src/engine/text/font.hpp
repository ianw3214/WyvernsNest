#pragma once

#include <GL/glew.h>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "../../math/vec.hpp"

struct Character {
    char       character;  // The character represented by this Character
    GLuint     textureID;  // ID handle of the glyph texture
    Vec2<int>  size;       // Size of glyph
    Vec2<int>  bearing;    // Offset from baseline to left/top of glyph
    GLuint     advance;    // Offset to advance to next glyph
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