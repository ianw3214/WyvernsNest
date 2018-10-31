#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "unit.hpp"

class Enemy : public Unit {

public:

	Enemy();
	~Enemy();

	void render();
	void drawHealth();
	void update();


	int id;

	bool isDead = false;

private:
	
	Sprite sprite;

};