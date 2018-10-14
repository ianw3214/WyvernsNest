#define SDL_MAIN_HANDLED

#include <iostream>
#include <exception>

#include "engine/core.hpp"
#include "game/combat.hpp"

int main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Core::init("Wyverns Nest", 1280, 720)) {
		std::cerr << "Initialization failed\n";
		return 1;
	}

	Combat * state = new Combat();
	Core::setState(state);

	// Main engine loop
	while (Core::running()) {
		try {
			Core::update();
		}
		catch(const std::exception& e) {
			std::cerr << e.what() << std::endl;
			throw;
		}
	}

	return 0;

}