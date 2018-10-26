#define SDL_MAIN_HANDLED

#include <iostream>

#include "engine/core.hpp"
#include "game/combat.hpp"
#include "game/skillTree.hpp"


int main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Core::init("Wyverns Nest", 1280, 720)) {
		std::cerr << "Initialization failed\n";
		return 1;
	}

	SkillTree * state = new SkillTree();
	Core::setState(state);

	// Main engine loop
	while (Core::running()) {
		Core::update();
	}

	return 0;

}