#pragma once

#include <string>
#include <vector>

#include "../../math/vec.hpp"
#include "unit.hpp"

class Combat;

// Enumeration for all attack types
enum class AttackType {
	MELEE,
	PLUS
};

// Use an integer to represent the AoE of the attack
using AttackAoE = int;

// Base class to represent an attack effect
class AttackEffect {
public:
	virtual void attack(ScreenCoord pos, Combat& combat) = 0;
};

// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
	DamageEffect() : damage(1) {}
	DamageEffect(int damage) : damage(damage) {}
	void attack(ScreenCoord pos, Combat& combat);

private:
	int damage;
};

// Generic attack class to hold all of the data for an attack
class Attack {

public:
	Attack(std::string name, 
		Unit * source, 
		AttackType type = AttackType::MELEE,
		AttackEffect * effect = new DamageEffect(), 
		AttackAoE aoe = 0);

	void attack(ScreenCoord pos, Combat& combat);
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
	Unit * source;

	AttackType type;
	AttackEffect * effect;
	AttackAoE aoe;
};

/*
class Melee : public Attack {
public:
	Melee() : 
		Attack(AttackType::MELEE),
		valid1("res/test8.png"),
		valid2("res/test8.png"),
		valid3("res/test8.png"),
		valid4("res/test8.png")
	{
		valid1.setSize(213, 180);
		valid2.setSize(213, 180);
		valid3.setSize(213, 180);
		valid4.setSize(213, 180);
	}

	int damage = 1;

	Sprite valid1;
	Sprite valid2;
	Sprite valid3;
	Sprite valid4;

	std::vector<ScreenCoord> getAttackPos(ScreenCoord pos) {
		//TODO returns all the positions hit by the attack
		std::vector<ScreenCoord> hit;
		if (isValid(pos)) {
			hit.push_back(pos);
		}

		isRendered = false;

		return hit;
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

	void showValidGrid() {
		int xScale = 213;
		int yScale = 180;
		valid1.setPos((playerPos.x()-1) * xScale, playerPos.y() * yScale);
		valid2.setPos(playerPos.x() * xScale, (playerPos.y()-1) * yScale);
		valid3.setPos((playerPos.x()+1) * xScale, playerPos.y() * yScale);
		valid4.setPos(playerPos.x() * xScale, (playerPos.y() + 1) * yScale);

		valid1.render();
		valid2.render();
		valid3.render();
		valid4.render();
	}


	void render(ScreenCoord pos) {
		if (isRendered) {
			showValidGrid();

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

class Plus : public Attack {
public:
	Plus() : 
		Attack(AttackType::MELEE),
		sprite1("res/test6.png"),
		sprite2("res/test6.png"),
		sprite3("res/test6.png"),
		sprite4("res/test6.png"),
		sprite5("res/test6.png"),
		sprite1_wrong("res/test7.png"),
		sprite2_wrong("res/test7.png"),
		sprite3_wrong("res/test7.png"),
		sprite4_wrong("res/test7.png"),
		sprite5_wrong("res/test7.png")

	{
		sprite1.setSize(213, 180);
		sprite2.setSize(213, 180);
		sprite3.setSize(213, 180);
		sprite4.setSize(213, 180);
		sprite5.setSize(213, 180);
		sprite1_wrong.setSize(213, 180);
		sprite2_wrong.setSize(213, 180);
		sprite3_wrong.setSize(213, 180);
		sprite4_wrong.setSize(213, 180);
		sprite5_wrong.setSize(213, 180);

	}

	int damage = 1;
	Sprite sprite1;
	Sprite sprite2;
	Sprite sprite3;
	Sprite sprite4;
	Sprite sprite5;

	Sprite sprite1_wrong;
	Sprite sprite2_wrong;
	Sprite sprite3_wrong;
	Sprite sprite4_wrong;
	Sprite sprite5_wrong;


	std::vector<ScreenCoord> getAttackPos(ScreenCoord pos) {
		//TODO returns all the positions hit by the attack
		std::vector<ScreenCoord> hit;
		if (isValid(pos)) {
			hit.push_back(pos);
			hit.push_back(pos + ScreenCoord(1,0));
			hit.push_back(pos + ScreenCoord(0, 1));
			hit.push_back(pos + ScreenCoord(-1, 0));
			hit.push_back(pos + ScreenCoord(0, -1));
		}

		isRendered = false;

		return hit;
	}

	bool isValid(ScreenCoord pos) {
		//TODO
		int x_dist = abs(pos.x() - playerPos.x());
		int y_dist = abs(pos.y() - playerPos.y());

		if (y_dist + x_dist >= 2) {
			return true;
		}
		return false;
	}

	void showValidGrid() {
		return;
	}

	void render(ScreenCoord pos) {
		if (isRendered) {
			if (isValid(pos)) {
				sprite1.setPos(pos.x() * 213, pos.y() * 180);
				sprite1.render();
				sprite2.setPos(pos.x() * 213, (pos.y() - 1) * 180);
				sprite2.render();
				sprite3.setPos((pos.x() + 1) * 213, pos.y() * 180);
				sprite3.render();
				sprite4.setPos(pos.x() * 213, (pos.y() + 1) * 180);
				sprite4.render();
				sprite5.setPos((pos.x() - 1) * 213, pos.y() * 180);
				sprite5.render();
			}
			else {
				//TODO values here are temporary
				sprite1_wrong.setPos(pos.x() * 213, pos.y() * 180);
				sprite1_wrong.render();
				sprite2_wrong.setPos(pos.x() * 213, (pos.y() - 1) * 180);
				sprite2_wrong.render();
				sprite3_wrong.setPos((pos.x() + 1) * 213, pos.y() * 180);
				sprite3_wrong.render();
				sprite4_wrong.setPos(pos.x() * 213, (pos.y() + 1) * 180);
				sprite4_wrong.render();
				sprite5_wrong.setPos((pos.x() - 1) * 213, pos.y() * 180);
				sprite5_wrong.render();
			}

		}
	}
};
*/