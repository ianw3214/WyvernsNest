#define SDL_MAIN_HANDLED

#include <iostream>

#include "engine/engine.hpp"

int main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Engine::get_instance().init("Wyverns Nest", 1280, 720)) {
		std::cout << "Initialization failed\n";
		return 1;
	}

	// Main engine loop
	while (Engine::get_instance().running()) {
		Engine::get_instance().update();
	}

	return 0;

}