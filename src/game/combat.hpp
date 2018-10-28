#pragma once

#include "../engine/core.hpp"
#include "combat/grid.hpp"
#include "combat/player.hpp"
#include "combat/enemy.hpp"


class Combat : public State {

public:

	Combat();
	~Combat();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	Player getPlayerAt(ScreenCoord at);
	int getIndexAt(ScreenCoord at);

	bool playerTurn = true;

	Grid grid;

	std::vector<Enemy> enemies;

	Player * selectedPlayer = nullptr;
	int selectedInt = -1;

private:

};