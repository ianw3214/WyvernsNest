#pragma once

#include <string>
#include "../../math/vec.hpp"

// Enumeration for all attack types
enum class AttackType {
	MELEE
};

class Attack {

public:
	Attack() {}
	Attack(AttackType type) : type(type) {}

	// Pure virtual function providing interface framework
	virtual ScreenCoord attack(ScreenCoord pos) = 0;
	virtual void showValidGrid(ScreenCoord playerPos, ScreenCoord grid) = 0;

	virtual bool isValid(ScreenCoord pos) = 0;

	bool isRendered = false;
	void toggleRender() {
		isRendered = !isRendered;
	};

	ScreenCoord playerPos;

	inline AttackType getType() const { return type; }

protected:
	std::string name;
	int damage;
	
private:
	AttackType type;

};


class Melee : public Attack {
public:
	Melee() : Attack(AttackType::MELEE) {}

	ScreenCoord attack(ScreenCoord pos) {
		//TODO returns all the positions hit by the attack
		isRendered = false;
		return pos;
	}

	bool isValid(ScreenCoord pos) {
		//TODO
		if ((pos.x() == playerPos.x() + 1 || pos.x() == playerPos.x() - 1) && pos.y() == playerPos.y()) {
			return true;
		}
		else if ((pos.y() == playerPos.y() + 1 || pos.y() == playerPos.y() - 1) && pos.x() == playerPos.x()) {
			return true;
		}
		return false;
	}

	void showValidGrid(ScreenCoord playerPos, ScreenCoord grid) {
		return;
	}


	void render(ScreenCoord pos) {
		if (isRendered) {
			if (isValid(pos)) {
				Sprite sprite("res/test6.png");
				//TODO values here are temporary
				sprite.setPos(pos.x() * 213, pos.y() * 180);
				sprite.setSize(213, 180);
				sprite.render();
			}
			else {
				Sprite sprite("res/test7.png");
				//TODO values here are temporary
				sprite.setPos(pos.x() * 213, pos.y() * 180);
				sprite.setSize(213, 180);
				sprite.render();
			}

		}
	}
};
