#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "attack.hpp"
#include "unit.hpp"

#define DEFAULT_SPRITE_WIDTH 100
#define DEFAULT_SPRITE_HEIGHT 100

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

	void setTileSize(int width, int height);
	std::vector<ScreenCoord> click(Vec2<int> to);
	void turnfOffAttacks();

	int id;

	PlayerAction current_action;
	Melee attack1;
	Plus attack2;
	int attackIndex = 0;

	ScreenCoord moveTarget;
	ScreenCoord moveDiff;
	ScreenCoord moveNext;


private:

	// State variables of the player
	int state_counter;

	// Variables needed for proper sprite rendering
	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;

	// Store both grid position and screen position to avoid recalculating every frame
	ScreenCoord screenPosition;
	void calculateScreenPosition();

	void calculateScreenPositionMovement();
	void incrementMovement();

	// Player sprites
	Sprite sprite_idle;
	Sprite sprite_selected;

};