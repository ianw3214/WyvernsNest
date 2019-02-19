#include "cursor.hpp"


Cursor::Cursor() : 
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png")
{
}


Cursor::~Cursor()
{
}

void Cursor::handleEvent(const SDL_Event& e) {
	// Update mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
	}
}

void Cursor::update(int delta) {
}

int Cursor::getMouseX() {
	return mouseX;
}

int Cursor::getMouseY() {
	return mouseY;
}

void Cursor::render() {
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	}
	else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}