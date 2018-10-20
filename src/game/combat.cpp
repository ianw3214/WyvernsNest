#include "combat.hpp"

Combat::Combat() { }

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {
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
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(0.0, 1.0, 0.0));
	Sprite sprite("res/test.png");
	sprite.setPos(100, 100);
	sprite.setSize(200, 200);
	sprite.render();

	std::string s;
	for (int c = 0; c < 20; ++c) {

		s.push_back(char(rand() % 127));
	}

	for (int i = 1; i < 25; ++i) {
		for (int j = 0; j < 4; ++j) {
			Core::Text_Renderer::render(s, ScreenCoord(j*300, i * 30));
		}
	}

}
