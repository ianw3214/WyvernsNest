#pragma once

#include "../engine/core.hpp"
#include "combat/grid.hpp"
#include "combat/unit.hpp"
#include "combat/player.hpp"
#include "combat/enemy.hpp"

#define GAME_OVER_MENU_WIDTH	800
#define GAME_OVER_MENU_HEIGHT	400

// TODO: Implement loading unit stats and calculating health, speed, etc. accordingly
// TODO: Base turn order off of unit stats
class Combat : public State {

public:

	Combat();
	~Combat();

	// Default state functions called from the state handler
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	// Utility function to get the unit at a grid position
	Unit * getUnitAt(ScreenCoord at);
	void nextUnitTurn();
	void selectUnit(Unit * unit);

	Grid grid;
	bool isPosEmpty(Vec2<int> pos) const;

private:

	// Combat state variables
	bool game_over;
	bool game_win;
	
	// Store a reference to all the units in the combat state
	std::vector<Unit*> units;

	// Reference to the current unit that is taking its action
	Unit * current;
	int unitIndex;

};