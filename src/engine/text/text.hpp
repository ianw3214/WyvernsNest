#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "font.hpp"
#include "../opengl/glwrappers.hpp"

enum TextAlignment { left, center, right };

class Text {
public:
    Text(const std::string& text, const std::string& fontPath, int size, Vec2<int> containerSize);

    void render(Vec3<float> colour, TextAlignment alignment);
private:
    // The characters making up the text
    std::vector<std::vector<Character>> m_lines;

    // The dimensions of the space in which this text will be rendered
    Vec2<int> m_containerSize;

    int m_lineCount;

    // Rendering
    Shader m_shader;
    GLuint m_vao, m_vbo;

    void initializeCharacters(Font& font, const std::string& text);
    void initializeOpenGLObjects();

    int getWidthOfLine(const std::vector<Character>& line);
};