#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

class Enemy {

public:

	Enemy();
	~Enemy();

	void render();
	void update();

	void takeDamage(int dmg);
	ScreenCoord getPosition();

	int id;

	ScreenCoord position = ScreenCoord(2, 2);
	int health = 10;

	int width = 25;
	int height = 50;

private:

};