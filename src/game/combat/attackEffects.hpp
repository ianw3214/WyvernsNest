#pragma once
#include "../unitData.hpp"

// NOTE: EFFECT IMPLEMENTATIONS FOUND IN `ATTACK.CPP` FILE

class Attack;

// The stat modifier for effects
struct EffectModifier {
	Stat stat;
	float modifier;

	EffectModifier() {}
	EffectModifier(Stat stat, float mod) : stat(stat), modifier(mod) {}
};

// Base class to represent an attack effect
class AttackEffect {
public:
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) = 0;
};

// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
	DamageEffect() : base_damage(1) {}
	DamageEffect(int damage) : base_damage(damage) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;

private:
	int base_damage;
};

// Attack effect that heals units
class HealEffect : public AttackEffect {
public:
	HealEffect() : heal(0) {}
	HealEffect(int heal) : heal(heal) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;

private:
	int heal;
};