#pragma once

#include <string>
#include <vector>

#include "../../math/vec.hpp"
#include "unit.hpp"

// Enumeration for all attack types
enum class AttackType {
	MELEE
};

// Use an integer to represent the AoE of the attack
using AttackAoE = int;

// Base class to represent an attack effect
class AttackEffect {
public:
	virtual void attack(ScreenCoord pos, std::vector<Unit*>& units) = 0;
};

// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
	DamageEffect() : damage(1) {}
	DamageEffect(int damage) : damage(damage) {}

	void attack(ScreenCoord pos, std::vector<Unit*>& units) {
		for (Unit * unit : units) {
			if (unit->position.x() == pos.x() && unit->position.y() == pos.y()) {
				// TODO: deal damage to the unit
			}
		}
	}

private:
	int damage;
};

// Generic attack class to hold all of the data for an attack
class Attack {

public:
	Attack(std::string name, 
		ScreenCoord source, 
		AttackType type = AttackType::MELEE,
		AttackEffect * effect = new DamageEffect(), 
		AttackAoE aoe = 0);

	void attack(ScreenCoord pos, std::vector<Unit*>& units);
	void showValidGrid(ScreenCoord playerPos, ScreenCoord grid);
	bool isValid(ScreenCoord pos);

	bool isRendered = false;
	void toggleRender() {
		isRendered = !isRendered;
	};

	// Getter functions for attack properties
	inline AttackType getType() const { return type; }
	inline const AttackEffect& getEffect() const { return *effect; }
	inline AttackAoE getAoE() const { return aoe; }
	
private:
	std::string name;
	ScreenCoord source;

	AttackType type;
	AttackEffect * effect;
	AttackAoE aoe;
};

/*
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
*/