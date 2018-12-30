#define SDL_MAIN_HANDLED

#include <iostream>

#include "engine/core.hpp"
#include "game/menus/menu.hpp"
#include "game/combat.hpp"

#include "engine/mixer.hpp"
#include "game/combat.hpp"
#include "game/customization.hpp"
#include "game/skillTree.hpp"

int SDL_main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Core::init("Wyverns Nest", 1280, 720)) {
		std::cerr << "Initialization failed\n";
		return 1;
	}

	Core::setDebugMode(true);

	// Combat * state = new Combat("res/data/levels/level1.json");
	// Combat * state = new Combat();
	// Customization * state = new Customization();
	// SkillTree * state = new SkillTree();
	Menu * state = new Menu();
	Core::setState(state);

	// Main engine loop
	while (Core::running()) {
		Core::update();
	}

	return 0;

}