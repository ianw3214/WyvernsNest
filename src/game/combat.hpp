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

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	Unit * getUnitAt(ScreenCoord at);
	std::vector<Enemy*> * getEnemiesAt(std::vector<ScreenCoord> * at);

	bool playerTurn = true;

	Grid grid;

private:

	// Store a reference to all the units in the combat state
	std::vector<Unit*> units;

	Unit * current;

};