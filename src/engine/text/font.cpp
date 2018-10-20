#include "font.hpp"

#include <iostream>

Font::Font(const std::string& fontPath, int size):
    m_textHeight(0) {
    if(FT_Init_FreeType(&m_fontLibrary)) {
        std::cerr << "Could not initialize FreeType library" << std::endl;
    }

    if(FT_New_Face(m_fontLibrary, fontPath.c_str(), 0, &m_fontFace)) {
        std::cerr << "Could not load font: " << fontPath << std::endl;
    }

    FT_Set_Pixel_Sizes(m_fontFace, 0, size);
}

Font::~Font() {
    FT_Done_Face(m_fontFace);
    FT_Done_FreeType(m_fontLibrary);
}

Character Font::generateCharacter(const char character) {
    // Check if the character has been generated previously
    auto iter = m_characters.find(character);
    if(iter != m_characters.end()) {
        // The character was generated previously
        return m_characters[character];
    } else {
        // Otherwise, let's generate the character.
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        int asciiCode = int(character);

        // Load the requested character.
        if(FT_Load_Char(m_fontFace, asciiCode, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load character: " << character << std::endl;
            // problems may occur if this happens and it isn't handled
        }

        // Generate a texture for the character.
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            GL_RED, 
            m_fontFace->glyph->bitmap.width, 
            m_fontFace->glyph->bitmap.rows, 
            0, 
            GL_RED, 
            GL_UNSIGNED_BYTE, 
            m_fontFace->glyph->bitmap.buffer
        );

        // Configure texture parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // Store the character.
        Character characterObj = {
            character,
            texture,
            Vec2<int>(m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows),
            Vec2<int>(m_fontFace->glyph->bitmap_left, m_fontFace->glyph->bitmap_top),
            static_cast<GLuint>(m_fontFace->glyph->advance.x)
        };

        m_characters.insert(std::pair<char, Character>(character, characterObj));

        return characterObj;
    }
}