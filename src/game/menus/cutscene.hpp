/**
  *		Cutscene game state
  *			- Set the next scene to transition to
  *			- Store a list of images(sprites) to show in the cutscene
  *			- Handle user input (mouse click or button press) to move between images
  *
  *		Future goals
  *			- Load cutscene data from file
  *			- Animated sprites in cutscenes
  *			- Text rendering in cutscenes
  *			- Sprite movement in cutscenes
  */
#pragma once

#include "../engine/core.hpp"
#include "util/util.hpp"
#include <ctime>
#include <string>
#include <GL/glew.h>
#include <iostream>
#include "vec.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

class Cutscene : public State {

public:

	// The constructor takes the next State to transition to as input
	Cutscene(State * state, std::string filePath, int scene_time = 5, bool timed_scene = false);
	~Cutscene();

	void handleEvent(const SDL_Event& e) override;
	
	void update(int delta) override;
	
	void render() override;
	
	void addSprite(std::string path);

	void addTextbox(std::string message);
	
	void getData();

	class Textbox {


	public:

		Textbox(std::string s, int posX, int posY);

		~Textbox();

		void render();

		//Just a guess for the width of each letter (glyph)
		const int  assumedWidth = 11.67;

		//Just a gues for the height of the text
		const int   assumedHeight = 43;

		//Text offset from the left hand side of the rectangle
		const int  xOffset = 9;

		//Number of character per line of the texttbox
		const int  CHARACTERS_PER_ROW = 50;

	private:

		//Vars for the dimensions
		float boxWidth, boxHeight;

		//More vars
		int xPos, yPos, rows;

		std::string myString;

		//Draws the 
		void drawRect();

		void renderText();

		//This function calculates the number of rows for the box based on the input length and then 
		//calculates the width of the box based on the maximum number of characters per row.
		void rowParser();

	};

private:
	
	bool m_timed_scene;
	
	State *m_state;

	int textBoxX = 50;

	int textBoxY = 525;
	
	std::clock_t m_start;
	
	int m_scene_time;
	
	std::string jsonFilePath;

	// State properties
	int mouseX, mouseY;
	
	bool mouseDown;

	// Utility sprites
	Sprite cursor;
	
	Sprite cursorPress;

	unsigned int m_position;
	//stores sprites
	std::vector<Sprite> m_sprites;
	//stores textbox messages
	std::vector<Textbox> textboxVector;

};

