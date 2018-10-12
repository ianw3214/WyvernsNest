#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "font.hpp"
#include "../opengl/glwrappers.hpp"

/**
 * Basic usage of Text
 * 
 * Create a Text object with the following parameters:
 *  - text: The text that will be rendered
 *  - fontPath: The path to the font used to render the text
 *  - size: The desired text size
 *  - containerSize: The size of the region in which text will be rendered
 * 
 * Displaying text (render method):
 *  - colour: The desired colour of the text
 *  - alignment: The desired alignment of the text (left, center, right)
 *  - location: The location for the text to be rendered. (0,0) is the top left corner of the window
 *  and (width, height) is the bottom right.
 * 
 * Example:
 *  Text text("this is some text", "res/test_font.ttf", 48, Vec2<int>(Core::windowWidth(), Core::windowHeight()))
 *  ...
 *  text.render(Vec3<float>(1, 1, 1), TextAlignment::center, ScreenCoord(150, 0));
 *
 * This creates a text object with the "test_font" font", "this is some text" as the text to be rendered, a font size
 * of 48 and a container that is the size of the window. The colour of the text is white, it is centered, and it begins
 * 150 pixels from the left side of the window and at the top of the window.
 **/

enum TextAlignment { left, center, right };

class Text {
public:
    Text(const std::string& text, const std::string& fontPath, int size, Vec2<int> containerSize);

    void render(Vec3<float> colour, TextAlignment alignment, ScreenCoord location);
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