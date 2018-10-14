#pragma once

#include "../engine/core.hpp"
#include "combat/grid.hpp"
#include "combat/player.hpp"


class Combat : public State {

public:

	Combat();
	~Combat();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	bool playerTurn = true;

	Grid grid;
	Player player;

	//Player selected = Player(1);

private:
	Sprite* m_testSprite;
};