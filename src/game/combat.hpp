#pragma once

#include "../engine/core.hpp"
#include "combat/grid.hpp"
#include "combat/unit.hpp"
#include "combat/player.hpp"
#include "combat/enemy.hpp"


class Combat : public State {

public:

	Combat();
	~Combat();

	// Default state functions called from the state handler
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void getNextUnit();
	void render() override;

	// Utility function to get the unit at a grid position
	Unit * getUnitAt(ScreenCoord at);
	void nextUnitTurn();
	void selectUnit(Unit * unit);

	bool playerTurn = true;

	Grid grid;

private:

	// Store a reference to all the units in the combat state
	std::vector<Unit*> units;

	// Reference to the current unit that is taking its action
	Unit * current;
	int unitIndex;

};