#pragma once

#include "../engine/core.hpp"

#define NUM_BUTTONS_SET 1

#define TITLE_TEXT	"SETTINGS"
#define VOLUME_TEXT	"VOLUME"
#define RES_TEXT	"RESOLUTION"
#define BACK_TEXT	"BACK"

#define NUM_OPTIONS 3

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
	Sprite cursor;
	Sprite cursorPress;
	Sprite slider;
	Sprite aspect;

	// Menu state
	int selected_option;
	int mouseX, mouseY;
	bool mouseDown;
};

