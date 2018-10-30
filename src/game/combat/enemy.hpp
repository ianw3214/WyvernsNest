#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "unit.hpp"

#define DEFAULT_SPRITE_WIDTH 100
#define DEFAULT_SPRITE_HEIGHT 100

class Enemy : public Unit {

public:

	Enemy();
	~Enemy();

	void render();
	void drawHealth();
	void update();

	void takeDamage(int dmg);
	ScreenCoord getPosition();

	void calculateScreenPosition();

	void setTileSize(int width, int height);

	int id;

	ScreenCoord screenPosition;

	int width = 25;
	int height = 50;

	bool isDead = false;

private:
	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;

	Sprite sprite;

};