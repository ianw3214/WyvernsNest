#pragma once
#include "../unitData.hpp"

// NOTE: EFFECT IMPLEMENTATIONS FOUND IN `ATTACK.CPP` FILE

class Attack;
class Status;

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
	DamageEffect(int damage = 0) : base_damage(damage) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;

private:
	int base_damage;
};

// Attack effect that heals units
class HealEffect : public AttackEffect {
public:
	HealEffect(int heal = 0) : heal(heal) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;

private:
	int heal;
};

class BurnEffect : public AttackEffect {
public:
	BurnEffect(int damage = 1, int ticks = 1, bool infinite = false) : 
		burn_damage(damage), ticks(ticks), infinite(infinite) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;
private:
	int burn_damage;
	int ticks;
	bool infinite;
};

class StatBuffEffect : public AttackEffect {
public:
	StatBuffEffect(Stat stat, float percent, int ticks, bool infinite = false) : 
		stat(stat), percent(percent), ticks(ticks), infinite(infinite) {}
	virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;
private:
	Stat stat;
	float percent;
	int ticks;
	bool infinite;
};

class PushEffect : public AttackEffect {
public:
	PushEffect(int distance = 1) : distance(distance) {}
	virtual void attack(ScreenCoord pos, Combat &combat, const Attack &attack) override;
private:
	int distance;
};

class MoveEffect : public AttackEffect {
public:
	MoveEffect() {}
	virtual void attack(ScreenCoord pos, Combat &combat, const Attack &attack) override;
private:
};

class BlinkEffect : public AttackEffect {
public:
	BlinkEffect() {}
	virtual void attack(ScreenCoord pos, Combat &combat, const Attack &attack) override;
private:
};

class ResurrectEffect : public AttackEffect {
public:
	ResurrectEffect() {}
	virtual void attack(ScreenCoord pos, Combat &combat, const Attack &attack) override;
};