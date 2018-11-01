#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "attack.hpp"
#include "unit.hpp"
#include "node.hpp"

#define PLAYER_DEFAULT_MOVE_COUNTER		20
#define PLAYER_DEFAULT_ATTACK_COUNTER	20

#define UI_X_OFFSET					   -10
#define UI_Y_OFFSET					    5
#define UI_OPTION_HEIGHT				30

// Enumeration to represent the possible player actions
enum class PlayerAction {
	NONE,
	MOVE,
	ATTACK_1,
	ATTACK_2
};

class Combat;

// TODO: update player so that the positions are calculated from the grid
class Player : public Unit {

public:

	Player();
	Player(int x, int y);
	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);
	void render();
	void renderTurnUI();
	void renderValidMoves();

	void click(Vec2<int> to, Combat& combat);
	void turnfOffAttacks();

	int id;

	// The action that is being expected from the player
	PlayerAction current_action;

	// Variables to help keep track of player movement
	ScreenCoord moveTarget;
	ScreenCoord moveDiff;
	ScreenCoord moveNext;

private:

	// Helper functions to calculate the screen position and movement of the player
	void calculateScreenPositionMovement();
	void incrementMovement();

	// Pathfinding helper methods
	void getPath(Combat & combat);
	std::vector<ScreenCoord> getValidNeighbours(ScreenCoord pos, Combat & combat);

	// Player sprites
	Sprite sprite_idle;
	Sprite sprite_selected;
	Sprite valid_tile;

};