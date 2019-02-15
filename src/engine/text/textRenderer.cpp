#include "textRenderer.hpp"
#include <algorithm>

TextRenderer::TextRenderer(const std::string& fontPath, int size, Vec2<int> windowSize) :
    m_shader("res/shaders/text_vertex.glsl", "res/shaders/text_fragment.glsl"), m_fontSize(size), m_windowSize(windowSize){
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

void TextRenderer::render(std::string s, ScreenCoord pos, float scale) {
	renderLine(s, pos, scale, m_colour, m_ha, m_va);
}

void TextRenderer::renderLine(std::string s, ScreenCoord pos, float scale, Colour colour, TextRenderer::hAlign ha, TextRenderer::vAlign va) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();
    m_shader.setUniform3f("textColour", colour.r(), colour.g(), colour.b());
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

	Vec2<float> sSize = computeLineSize(s, scale);
	GLfloat x_begin;
	switch (ha) {
	case TextRenderer::hAlign::left:
		x_begin = static_cast<GLfloat>(pos.x());
		break;
	case TextRenderer::hAlign::centre:
		x_begin = static_cast<GLfloat>(pos.x() - scale*sSize.x() / 2.f);
		break;
	case TextRenderer::hAlign::right:
		x_begin = static_cast<GLfloat>(pos.x() - scale*sSize.x());
		break;
	}
	
	GLfloat y_begin;
	switch (va) {
	case TextRenderer::vAlign::bottom:
		y_begin = static_cast<GLfloat>(pos.y());
		break;
	case TextRenderer::vAlign::middle:
		y_begin = static_cast<GLfloat>(pos.y() + scale*sSize.y() / 2.f);
		break;
	case TextRenderer::vAlign::top:
		y_begin = static_cast<GLfloat>(pos.y() + scale*sSize.y());
		break;
	}

	GLfloat x = static_cast<GLfloat>(x_begin);
	GLfloat y = -static_cast<GLfloat>(y_begin);	// Sign error somewhere below perhaps.
	

    for(char c : s) {
		Character ch = m_text[c];
        GLfloat xpos = static_cast<GLfloat>(x + ch.bearing.x()*scale);
        GLfloat ypos = static_cast<GLfloat>(y + ch.bearing.y()*scale);

        GLfloat w = static_cast<GLfloat>(ch.size.x()) * scale;
        GLfloat h = static_cast<GLfloat>(ch.size.y()) * scale;
			
		int winHeight = m_windowSize.y();
        // Update VBO for each character
        GLfloat vertices[] = {
			// (winHeight - h) makes the text start from the top left corner 
             xpos,     ypos + h + (winHeight - h),   0.0, 0.0 ,
             xpos,     ypos + (winHeight - h),       0.0, 1.0 ,
             xpos + w, ypos + (winHeight - h),       1.0, 1.0 ,

             xpos,     ypos + h + (winHeight - h),   0.0, 0.0 ,
             xpos + w, ypos + (winHeight - h),       1.0, 1.0 ,
             xpos + w, ypos + h + (winHeight - h),   1.0, 0.0
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
        x += (ch.advance >> 6)*scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shader.unbind();
}

Vec2<float> TextRenderer::computeLineSize(std::string s, float scale) {
	int x = 0;
	for (char c : s) {
		Character ch = m_text[c];
		x += (ch.advance >> 6);	// Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	int y = m_fontSize;
	return Vec2<float>(float(x), float(y));
}