#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "font.hpp"
#include "../opengl/glwrappers.hpp"

class Text {
public:
    Text(const std::string& fontPath, int size);

    void render(std::string s, ScreenCoord pos);
private:
    std::vector<Character> m_text;

    Shader shader;
    GLuint VAO, VBO;
};