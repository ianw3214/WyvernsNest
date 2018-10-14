#include "combat.hpp"

Combat::Combat() {
	m_testSprite = ResourceEngine::getSprite("test");
	m_testSprite->setPos(100, 100);
	m_testSprite->setSize(200, 200);
}

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
	m_testSprite->render();
}
