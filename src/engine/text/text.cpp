#include "text.hpp"

const char* text_vertex =
    "#version 330 core\n"
	"layout(location = 0) in vec4 position; /* vec2 position, vec2 texCoord */"
    "out vec2 texCoord;"
    "uniform vec2 containerSize; /* width, height */"
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
    "uniform sampler2D textTexture;"
    "uniform vec3 textColour;"
	"void main() {"
		"colour = vec4(textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(textTexture, texCoord).r);"
	"}";

Text::Text(const std::string& text, const std::string& fontPath, int fontSize, Vec2<int> containerSize) :
    m_shader(text_vertex, text_fragment),
    m_containerSize(containerSize),
    m_lineCount(1) {
    Font font(fontPath, fontSize);

    initializeCharacters(font, text);
    initializeOpenGLObjects();    
}

#include <iostream>

void Text::render(Vec3<float> colour, TextAlignment alignment) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();
    m_shader.setUniform2i("containerSize", m_containerSize.x(), m_containerSize.y());
    m_shader.setUniform3f("textColour", colour.r(), colour.g(), colour.b());
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    GLfloat XcursorPosition = 0, YcursorPosition = 0;
    int lineHeight = 0;
    for(std::vector<Character> line: m_lines) {
        int Xoffset = 0;
        switch(alignment) {
            case left:
                break;
            case center:
                Xoffset = (m_containerSize.x() - getWidthOfLine(line)) / 2.0f;
                break;
            case right:
                Xoffset = (m_containerSize.x() - getWidthOfLine(line));
                break;
        }
        for(Character ch: line) {
            GLfloat xpos = Xoffset + XcursorPosition + ch.bearing.x();
            GLfloat ypos =  ch.bearing.y() - ch.size.y() - YcursorPosition;

            lineHeight = ch.size.y();

            GLfloat w = ch.size.x();
            GLfloat h = ch.size.y();

            int containerHeight = m_containerSize.y();

            // Update VBO for each character
            GLfloat vertices[] = {
                // (containerHeight - h) makes the text start from the top left corner
                xpos, ypos + containerHeight, 0.0, 0.0,            
                xpos, ypos + (containerHeight - h), 0.0, 1.0,
                xpos + w, ypos + (containerHeight - h), 1.0, 1.0,

                xpos, ypos + containerHeight, 0.0, 0.0,
                xpos + w, ypos + (containerHeight - h), 1.0, 1.0,
                xpos + w, ypos + containerHeight, 1.0, 0.0            
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            XcursorPosition += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        XcursorPosition = 0;
        YcursorPosition += lineHeight;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shader.unbind();
}

void Text::initializeCharacters(Font& font, const std::string& text) {
    std::vector<Character> currentLine;
    int currentLineWidth = 0;

    for(const char& c: text) {
        if(c == '\n') {
            // we have a newline
            currentLineWidth = 0;

            m_lines.push_back(currentLine);
            currentLine.clear();
            continue;
        }

        Character character = font.generateCharacter(c);
        int advance = (character.advance >> 6);
        if(advance + currentLineWidth > m_containerSize.x()) {
            // we need a new line
            currentLineWidth = 0;

            m_lines.push_back(currentLine);
            currentLine.clear();
        }

        currentLine.push_back(character);
        currentLineWidth += advance;
    }

    m_lines.push_back(currentLine);
}

void Text::initializeOpenGLObjects() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  
}

int Text::getWidthOfLine(const std::vector<Character>& line) {
    int total = 0;
    for(Character c: line) {
        total += (c.advance >> 6);
    }
    return total;
}