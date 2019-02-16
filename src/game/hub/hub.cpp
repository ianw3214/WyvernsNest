#include "hub.hpp"

#include <utility>
#include <fstream>

Hub::Hub(const std::string& filePath) : backdrop(filePath)
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
	// TODO
}

void Hub::update(int delta) {
	// TODO
}

void Hub::render() {
	backdrop.render();
}