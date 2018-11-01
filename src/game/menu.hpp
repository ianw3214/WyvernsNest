#pragma once

#include "../engine/core.hpp"

#define FLASH_TIME	20

class Menu : public State {

public:

	Menu();
	~Menu();

	void handleEvent(const SDL_Event& e);   // Handles incoming events
	void update(int delta);                 // Handles state logic
	void render();                          // Handles entity rendering

private:
	Sprite background;

	// Counter for flashing text
	int counter;
	bool render_text;
};