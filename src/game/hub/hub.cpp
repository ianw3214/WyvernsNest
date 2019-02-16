#include "hub.hpp"

#include <utility>
#include <fstream>

Hub::Hub(const std::string& filePath) :
	backdrop(filePath),
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
	player.handleEvent(e);
}

void Hub::update(int delta) {
	player.update(delta);
}

void Hub::render() {
	Core::Renderer::clear();
	backdrop.render();
	player.render();
}
