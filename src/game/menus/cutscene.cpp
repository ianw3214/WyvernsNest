#include "cutscene.hpp"
#include <iostream>
#include <ctime>
#include <math.h>

Cutscene::Cutscene(State * state, std::string filePath, int scene_time, bool timed_scene) :
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png")
{
	m_scene_time = scene_time;
	m_position = 0;
	m_state = state;
	m_start = std::clock();
	m_timed_scene = timed_scene;
	jsonFilePath = filePath;
	std::cout << jsonFilePath << "\n";
	getData();

}

Cutscene::~Cutscene() {

}

void Cutscene::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_SPACE && m_position < m_sprites.size() - 1) {
			m_position++;
		}
		else if (e.key.keysym.sym == SDLK_SPACE && m_position == m_sprites.size() - 1) {
			changeState(m_state);
		}
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			// Skip the cutscene if the escape key is pressed
			changeState(m_state);
		}
	}
	// Also cycle through the image on mouse press
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (m_position == m_sprites.size() - 1) changeState(m_state);
		else m_position++;
	}
	// Update the mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
	}
}

void Cutscene::update(int delta) {
	if (((std::clock() - m_start) / (double)CLOCKS_PER_SEC) > m_scene_time && m_timed_scene) {
		if (m_position >= m_sprites.size() - 1) {
			changeState(m_state);
		}
		else {
			m_start = std::clock();
		}
	}
}

void Cutscene::render() {

	m_sprites[m_position].render();
	textboxVector[m_position].render();

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	}
	else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void Cutscene::addSprite(std::string path) {
	m_sprites.push_back(Sprite(path));
	m_sprites[m_sprites.size() - 1].setSize(Core::windowWidth(), Core::windowHeight());
}

void Cutscene::getData() {

	std::ifstream jsonFile(Cutscene::jsonFilePath);

	nlohmann::json data;

	jsonFile >> data;

	//Get json data from the file.
	//Add a textbox with a corresponding message from the file.
	//Add a sprite to the sprite vector (array list).

	for (const nlohmann::json& slide : data["slides"])
	{

		addSprite(slide["spritePath"]);
		addTextbox(slide["textBoxMessages"]);

	}

}

void Cutscene::addTextbox(std::string message) {

	//Add textbox to the arraylist.
	textboxVector.push_back(Textbox(message, Cutscene::textBoxX, Cutscene::textBoxY));
}

Cutscene::Textbox::Textbox(std::string s, int x, int y)
{
	myString = s;
	xPos = x;
	yPos = y;
	
	//debug only
	//std::cout << "Hello, my name is " << this << ", and I am a textbox :)" << "\n";	
	
	rowParser();
}

Cutscene::Textbox::~Textbox()
{
	//Debug only
	//std::cout << "A textbox named " << this << " was destroyed :(" << "\n";
}

void Cutscene::Textbox::drawRect()
{
	//Render rectanlge using core api
	Core::Renderer::drawRect(ScreenCoord(xPos, yPos), Textbox::boxWidth, Textbox::boxHeight, Colour(.0f, .0f, .0f));
}

void Cutscene::Textbox::renderText()
{
	//Aesthetics
	Core::Text_Renderer::setColour(Colour(255.0f, .0f, .0f));
	
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	
	int i = 0;
	//Rendering each line of the textbox
	for (; i < rows; i++) {
		Core::Text_Renderer::render(myString.substr(Textbox::CHARACTERS_PER_ROW*i, CHARACTERS_PER_ROW), ScreenCoord(xPos + Textbox::xOffset, yPos + Textbox::assumedHeight*i), 1.0f);
	}

	//std::cout << Textbox::CHARACTERS_PER_ROW*i << " "<< Textbox::CHARACTERS_PER_ROW*(i + 1) << " " << myString.substr(Textbox::CHARACTERS_PER_ROW*i, Textbox::CHARACTERS_PER_ROW) << "\n";
	//Core::Text_Renderer::render(myString.substr(Textbox::CHARACTERS_PER_ROW*i, Textbox::CHARACTERS_PER_ROW), ScreenCoord(xPos + Textbox::xOffset, yPos + Textbox::assumedHeight*i), 1.0f);
	//Resetting colour
	Core::Text_Renderer::setColour(Colour(.0f, .0f, .0f));
}

void Cutscene::Textbox::rowParser()
{
	//Calculating the number of rows for the textbox by dividing the message by the maximum character per row.
	rows = ceil(Textbox::myString.length() / Textbox::CHARACTERS_PER_ROW) + 1;

	//Calculating the width of the box multiplying maximum number of character by the width of each character
	//If only one row is in the textbox then we just multiply the character width by the  number of characters in the message string.
	boxWidth = (rows > 1) ? Textbox::CHARACTERS_PER_ROW * Textbox::assumedWidth : Textbox::assumedWidth * Textbox::myString.length();

	//Calculating box height
	boxHeight = Textbox::rows * Textbox::assumedHeight;

}

void Cutscene::Textbox::render()
{
	//Draw the background rectangle and then render the text on top of it.
	drawRect();
	renderText();
}
