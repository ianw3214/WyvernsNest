#pragma once

#include <string>
#include "../../math/vec.hpp"

class Attack {

public:
	// pure virtual function providing interface framework.

	virtual ScreenCoord attack(ScreenCoord pos) = 0;
	virtual void showValidGrid(ScreenCoord playerPos, ScreenCoord grid) = 0;

	virtual bool isValid(ScreenCoord pos, ScreenCoord grid) = 0;

	bool isRendered = false;
	void toggleRender() {
		isRendered = !isRendered;
	};

protected:
	std::string name;
	int damage;
	//int AOE[10];

};


class Melee : public Attack {
public:

	ScreenCoord attack(ScreenCoord pos) {
		//TODO returns all the positions hit by the attack
		isRendered = false;
		return pos;
	}

	bool isValid(ScreenCoord pos, ScreenCoord grid) {
		//TODO
		return true;
	}

	void showValidGrid(ScreenCoord playerPos, ScreenCoord grid) {
		return;
	}


	void render(ScreenCoord pos) {
		if (isRendered) {
			Sprite sprite("res/test5.png");
			//TODO values here are temporary
			sprite.setPos(pos.x() * 213, Core::windowHeight() - pos.y() * 180 - 100);
			sprite.setSize(100, 100);
			sprite.render();
		}
	}
};
