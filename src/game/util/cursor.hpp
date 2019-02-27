#pragma once

#include "../../engine/core.hpp"

class Cursor : public Entity
{
public:
	Cursor();
	~Cursor();

	// Override entity functions
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	// Position getter
	int getMouseX();
	int getMouseY();

private:
	// Mouse position
	int mouseX, mouseY;

	// Mouse press flag
	bool mouseDown = false;

	// Sprites
	Sprite cursor;
	Sprite cursorPress;
};

