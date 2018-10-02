#include "combat.hpp"

Combat::Combat() {

}

Combat::~Combat() {

}

void Combat::handleEvent(SDL_Event e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_SPACE) {
			// Do something when space is pressed
		}
	}
}

void Combat::update(int delta) {
	// Update shit here
}

void Combat::render() {

	// SAMPLE CODE
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));
	Sprite sprite("res/test.png");
	sprite.setPos(100, 100);
	sprite.setSize(100, 100);
	sprite.render();

}
