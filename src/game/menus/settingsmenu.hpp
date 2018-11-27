#pragma once

#include "../engine/core.hpp"

#define NUM_BUTTONS_SET 1

class SettingsMenu : public State {

public:
	SettingsMenu();
	~SettingsMenu();

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

	int getButtonIndexAtPos(ScreenCoord coord);
	void switchToCurrentState();
};

