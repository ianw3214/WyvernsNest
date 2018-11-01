#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "unit.hpp"

#define ENEMY_DEFAULT_ATTACK_COUNTER 20

class Enemy : public Unit {

public:

	Enemy();
	~Enemy();

	void render();
	void drawHealth();
	void update(int delta);

	void takeTurn();

	int id;

	bool isDead = false;

private:
	
	Sprite sprite;

};