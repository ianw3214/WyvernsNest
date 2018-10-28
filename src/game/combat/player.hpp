#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "attack.hpp"

#define DEFAULT_SPRITE_WIDTH 100
#define DEFAULT_SPRITE_HEIGHT 100

// Enumeration of all player states
enum class PlayerState {
	IDLE,
	MOVE
};

// TODO: update player so that the positions are calculated from the grid
class Player : public Entity {

public:

	Player();
	Player(int x, int y);
	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);
	void render();

	inline PlayerState getState() const { return state; }

	void setTileSize(int width, int height);
	ScreenCoord move(Vec2<int> to);

	int id;

	Vec2<int> position;
	bool selected = false;

	Melee attack1;
	int attackIndex = 0;

	ScreenCoord moveTarget;
	ScreenCoord moveDiff;
	ScreenCoord moveNext;


private:

	// State variables of the player
	PlayerState state;
	int state_counter;

	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;

	// Store both grid position and screen position to avoid recalculating every frame
	ScreenCoord screenPosition;
	void calculateScreenPosition();

	void calculateScreenPositionMovement();
	void incrementMovement();

	Sprite idle;

};