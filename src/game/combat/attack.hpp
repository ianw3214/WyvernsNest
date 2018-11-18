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
	PIERCE
};

// Use an integer to represent the range of the attack
using AttackRange = int;

// Use an integer to represent the AoE of the attack
using AttackAoE = int;

#include "attackEffects.hpp"

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

	// The main method to execute the attack at a certain point
	void attack(ScreenCoord pos, Combat& combat);

	// Attack modifier functions
	void addEffectModifier(EffectModifier modifier);
	void addEffectModifier(Stat stat, float multiplier);

	// Utility methods
	void renderValidGrid(int tile_width, int tile_height);
	void renderValidTarget(int tile_width, int tile_height);
	bool isValid(ScreenCoord pos);
	const Unit * getSource() const { return source; }

	// Getter functions for attack properties
	AttackType getType() const { return type; }
	const AttackEffect& getEffect() const { return *effect; }
	AttackAoE getAoE() const { return aoe; }
	std::string getName() const { return name; }
	const std::vector<EffectModifier>& getEffectModifiers() const { return effectModifiers; }

private:
	// Attack identifiers
	std::string name;
	Unit * source;

	// Attack properties
	AttackType type;
	AttackRange range;
	AttackEffect * effect;
	AttackAoE aoe;
	bool affect_self;

	// Attack modifiers
	std::vector<EffectModifier> effectModifiers;

	// Common attack sprites
	Sprite validSprite;
	Sprite targetValidSprite;
	Sprite targetInvalidSprite;

	// Helper methods
	void attackAoE(ScreenCoord pos, Combat& combat);
};