#pragma once

#include "../engine/core.hpp"

#include <vector>
#include <string>

#define FLASH_TIME	20

// Use a hard coded define for the version for now, in the future use a config file
#define VERSION "1.0.1 ALPHA"

// Hard coded button values
#define NUM_BUTTONS 4

// Other hard coded values
// TODO: unify these variable locations somewhere, perhaps in a config file
#define USER_SAVE_LOCATION		"res/data/save.json"
#define MASTER_LEVEL_LOCATION	"res/data/levels/master.json"

#define DEFAULT_PLAYER_ATTACK_1	"PUNCH"
#define DEFAULT_PLAYER_ATTACK_2	"THROW ROCK"
#define DEFAULT_PLAYER_ATTACK_3	"DASH"
#define DEFAULT_PLAYER_ATTACK_4	"SPEED BOOST"

class Menu : public State {

public:

	Menu(bool start_music = true);
	~Menu();

	void handleEvent(const SDL_Event& e);   // Handles incoming events
	void update(int delta);                 // Handles state logic
	void render();                          // Handles entity rendering

private:
	// Sprites
	Sprite background;
	Sprite highlight;
	Sprite cursor;
	Sprite cursorPress;

	// Menu state
	int selected_option;
	int mouseX, mouseY;
	bool mouseDown;

	// Counter for flashing text
	int counter;
	bool render_text;

	// The buttons to move to next states
	std::vector<std::string> buttons;
	std::vector<ScreenCoord> buttonCoords;

	// Other helper methods
	void switchToCurrentState();
	void changeToCombatState();
	void initializeSaveFile();
	int getButtonIndexAtPos(ScreenCoord coord);
};