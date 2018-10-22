#pragma once

#include <string>
#include "../../math/vec.hpp"
#include "grid.hpp"

class Attack {

public:
	// pure virtual function providing interface framework.

	virtual void attack(ScreenCoord pos, Grid grid) = 0;
	virtual void showValidGrid(ScreenCoord playerPos, Grid grid) = 0;

	virtual bool isValid(ScreenCoord pos, Grid grid) = 0;

protected:
	std::string name;
	int damage;
	//int AOE[10];

};


class Melee : public Attack {
public:

	void attack(ScreenCoord pos, Grid grid) {
		return;
	}

	bool isValid(ScreenCoord pos, Grid grid) {
		//TODO
		return true;
	}
};
