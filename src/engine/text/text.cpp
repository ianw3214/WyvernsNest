#include "text.hpp"

#include <iostream>

const char* text_vertex =
    "#version 330 core\n"
	"layout(location = 0) in vec4 position;"
    "out vec2 texCoord;"
    "mat4 ortho(float left, float right, float bottom, float top) {"
    "return mat4(vec4(2.0 / (right - left), 0, 0, 0), vec4(0, 2.0 / top - bottom, 0, 0), vec4(0, 0, -1, 0), vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), 0, 1));}"
	"void main() {"
		"gl_Position = ortho(0, 1280, 0, 720) * vec4(position.xy, 0, 1);"
        "texCoord = position.zw;"
	"}";

const char* text_fragment =
    "#version 330 core\n"
    "in vec2 texCoord;"
	"out vec4 colour;"
    "uniform sampler2D text;"
    "uniform vec3 textColour;"
	"void main() {"
		"colour = vec4(textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, texCoord).r);"
	"}";

Text::Text(const std::string& fontPath, int size) :
    shader(text_vertex, text_fragment), fontSize(size) {
    Font font(fontPath, size);

	m_text.resize(128);
	for (char c = 0; c < 127; ++c) {
		m_text[c] = font.generateCharacter(c);
	}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::render(std::string s, ScreenCoord pos, Text::hAlign ah, Text::vAlign av) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.bind();
    shader.setUniform3f("textColour", 1, 0, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

	Vec2<int> sSize = stringSize(s);
	GLfloat x_begin;
	switch (ah) {
	case Text::hAlign::left:
		x_begin = pos.x();
		break;
	case Text::hAlign::centre:
		x_begin = pos.x() - sSize.x() / 2;
		break;
	case Text::hAlign::right:
		x_begin = pos.x() - sSize.x();
		break;
	}
	
	GLfloat y_begin;
	switch (av) {
	case Text::vAlign::top:
		y_begin = pos.y();
		break;
	case Text::vAlign::middle:
		y_begin = pos.y() - sSize.y() / 2;
		break;
	case Text::vAlign::bottom:
		y_begin = pos.y() - sSize.y();
		break;
	}

	GLfloat x = static_cast<GLfloat>(x_begin);
	GLfloat y = -static_cast<GLfloat>(y_begin);	// Sign error somewhere below...
	

    for(char c : s) {
		Character ch = m_text[c];
        GLfloat xpos = static_cast<GLfloat>(x + ch.bearing.x());
        GLfloat ypos = static_cast<GLfloat>(y + ch.bearing.y() - ch.size.y());

        GLfloat w = static_cast<GLfloat>(ch.size.x());
        GLfloat h = static_cast<GLfloat>(ch.size.y());
        // Update VBO for each character
        GLfloat vertices[] = {
             xpos,     ypos + h + (720 - h),   0.0, 0.0 ,            
             xpos,     ypos + (720 - h),       0.0, 1.0 ,
             xpos + w, ypos + (720 - h),       1.0, 1.0 ,

             xpos,     ypos + h + (720 - h),   0.0, 0.0 ,
             xpos + w, ypos + (720 - h),       1.0, 1.0 ,
             xpos + w, ypos + h + (720 - h),   1.0, 0.0            
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shader.unbind();
}

Vec2<int> Text::stringSize(std::string s) {
	int x = 0;
	for (char c : s) {
		Character ch = m_text[c];
		x += (ch.advance >> 6);	// Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	int y = fontSize;
	return Vec2<int>(x, y);
}