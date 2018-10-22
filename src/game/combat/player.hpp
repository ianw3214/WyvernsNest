#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

#define DEFAULT_SPRITE_WIDTH 100
#define DEFAULT_SPRITE_HEIGHT 100

// TODO: update player so that the positions are calculated from the grid
class Player : public Entity {

public:

	Player();
	Player(int x, int y);
	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);
	void render();

	void setTileSize(int width, int height);
	void move(Vec2<int> to);

	int id;

	Vec2<int> position;
	bool selected = false;

private:

	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;

	// Store both grid position and screen position to avoid recalculating every frame
	ScreenCoord screenPosition;
	void calculateScreenPosition();

	Sprite idle;

};