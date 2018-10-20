#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "font.hpp"
#include "../opengl/glwrappers.hpp"

class Text {
public:
	enum hAlign { left, centre, right };
	enum vAlign { top, middle, bottom };

    Text(const std::string& fontPath, int size);

    void render(std::string s, ScreenCoord pos, hAlign ha = hAlign::left, vAlign va = vAlign::top);
private:
    std::vector<Character> m_text;
	Vec2<int> stringSize(std::string s);

	int fontSize;
    Shader shader;
    GLuint VAO, VBO;
};