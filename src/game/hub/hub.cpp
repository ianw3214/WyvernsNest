#include "hub.hpp"

#include "util/util.hpp"

#include <utility>
#include <fstream>

Hub::Hub(const std::string& filePath) :
	backdrop(filePath),
	pauseMenu(),
	cursor(),
	player("res/assets/shadow.png", Core::windowWidth() / 2, Core::windowHeight() / 2)
{
	initSprites();
}


Hub::~Hub()
{
}

void Hub::initSprites() {
	// resize sprites
	backdrop.setSize(Core::windowWidth(), Core::windowHeight());
}

void Hub::handleEvent(const SDL_Event& e) {
	if (!pauseMenu.getIsShown()) {
		player.handleEvent(e);
	}

	// Handle pause menu
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
		pauseMenu.setIsShown(true);
	}
	if (pauseMenu.getIsShown()) {
		// Handle cursor events
		cursor.handleEvent(e);

		pauseMenu.handleEvent(e);
	}
}

void Hub::update(int delta) {
	if (!pauseMenu.getIsShown()) player.update(delta);
	pauseMenu.update(delta);
}

void Hub::render() {
	Core::Renderer::clear();
	backdrop.render();
	player.render();
	pauseMenu.render();
	// Only render the cursor when pausing
	if (pauseMenu.getIsShown()) cursor.render();
}
