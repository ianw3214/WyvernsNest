#pragma once

#include <string>
#include <vector>

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

class Combat;
class Unit;

// Enumeration for all attack types
enum class AttackType {
	INVALID,	// THIS SHOULD NEVER BE A REAL ATTACK, ONLY USED IF SOMETHING FAILS
	SELF,
	MELEE,
	RANGED,
	PIERCE,
	SPIN,
	HEAL,
	BIG_AOE
};

// Use an integer to represent the range of the attack
using AttackRange = int;

// Use an integer to represent the AoE of the attack
using AttackAoE = int;

// TODO: Move attack effects to a different file location
// Base class to represent an attack effect
class AttackEffect {
public:
	virtual void attack(ScreenCoord pos, Combat& combat, int stat, double mult) = 0;
};

// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
	DamageEffect() : damage(1) {}
	DamageEffect(int damage) : damage(damage) {}
	void attack(ScreenCoord pos, Combat& combat, int stat, double mult);
	int damage;

private:
};

// TODO: Handle invalid attacks during gameplay
// TODO: Change affect self to be calculated in the effect, not in the attack
// TODO: Change attacks to be able to have multiple effects
// Generic attack class to hold all of the data for an attack
class Attack {

public:
	Attack();
	Attack(std::string name, 
		Unit * source, 
		AttackType type = AttackType::MELEE,
		AttackRange range = 0,
		AttackEffect * effect = new DamageEffect(), 
		AttackAoE aoe = 0,
		bool affect_self = false);
	// Constuctor that duplicates an attack but changes the source unit
	Attack(const Attack& other, Unit * source);

	void attack(ScreenCoord pos, Combat& combat);
	void renderValidGrid(int tile_width, int tile_height);
	void renderValidTarget(int tile_width, int tile_height);
	bool isValid(ScreenCoord pos);

	// Getter functions for attack properties
	inline AttackType getType() const { return type; }
	inline const AttackEffect& getEffect() const { return *effect; }
	inline AttackAoE getAoE() const { return aoe; }
	inline std::string getName() const { return name; }

private:
	std::string name;
	Unit * source;

	AttackType type;
	AttackRange range;
	AttackEffect * effect;
	AttackAoE aoe;
	bool affect_self;

	Sprite validSprite;
	Sprite targetValidSprite;
	Sprite targetInvalidSprite;

	// Helper methods
	void attackAoE(ScreenCoord pos, Combat& combat);
};