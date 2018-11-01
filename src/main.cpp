#define SDL_MAIN_HANDLED

#include <iostream>

#include "engine/core.hpp"
#include "game/menu.hpp"

int main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Core::init("Wyverns Nest", 1280, 720)) {
		std::cerr << "Initialization failed\n";
		return 1;
	}

	// Core::setDebugMode(true);

	// Combat * state = new Combat();
	Menu * state = new Menu();
	Core::setState(state);

	// Main engine loop
	while (Core::running()) {
		Core::update();
	}

	return 0;

}