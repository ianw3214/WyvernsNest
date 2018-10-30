#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "attack.hpp"
#include "unit.hpp"

enum class PlayerAction {
	NONE,
	MOVE,
	ATTACK_1,
	ATTACK_2
};

// TODO: update player so that the positions are calculated from the grid
class Player : public Unit {

public:

	Player();
	Player(int x, int y);
	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);
	void render();

	void click(Vec2<int> to, Combat& combat);
	void turnfOffAttacks();

	int id;

	PlayerAction current_action;

	ScreenCoord moveTarget;
	ScreenCoord moveDiff;
	ScreenCoord moveNext;


private:

	// State variables of the player
	int state_counter;

	void calculateScreenPositionMovement();
	void incrementMovement();

	// Player sprites
	Sprite sprite_idle;
	Sprite sprite_selected;

};