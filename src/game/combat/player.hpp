#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

class Player : public Entity{

public:

	Player();
	~Player();
		
	void render();
	void update();


	void move(ScreenCoord to);

	int id;
	ScreenCoord position;
	int width = 25;
	int height = 50;

	bool selected = false;

private:
	Sprite* m_test4;
	Sprite* m_test5;
};