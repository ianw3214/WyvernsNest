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

class TextRenderer {
public:

	enum hAlign { left, centre, right };
	enum vAlign { top, middle, bottom };

    TextRenderer(const std::string& fontPath, int size, Vec2<int> windowSize);

	void render(std::string s, ScreenCoord pos, float scale = 1.f);
	inline void setAlignment(hAlign ha, vAlign va) { m_ha = ha; m_va = va; }
	inline void setColour(Colour colour) { m_colour = colour; }

private:

	void renderLine(std::string s, ScreenCoord pos, float scale, Colour colour = Colour(0, 0, 0), hAlign ha = hAlign::left, vAlign va = vAlign::top);
    std::vector<Character> m_text;
	Vec2<int> computeLineSize(std::string s, float scale);

	int m_fontSize;
	Vec2<int> m_windowSize;
    Shader m_shader;
    GLuint VAO, VBO;

	hAlign m_ha = hAlign::left;
	vAlign m_va = vAlign::top;
	Colour m_colour = Colour(0,0,0);
};