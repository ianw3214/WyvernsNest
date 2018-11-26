#pragma once

#include "../unitData.hpp"

class Unit;

enum class StatusType {
	BURN,
	STAT_BUFF
};

class Status {
public:
	Status(StatusType type, int turns, bool infinite, Unit * target) :
		type(type), 
		turns(turns), 
		target(target), 
		infinite(infinite) {}
	bool tick();

	void setTarget(Unit * unit) { target = unit; }

	// Modifier methods for status that may modify attributes
	virtual float getStatModifier(Stat stat) const { return 0.f; }
protected:
	virtual void affectUnit() = 0;
	Unit * target;
private:
	StatusType type;
	int turns;
	bool infinite;
};

class BurnStatus : public Status {
public:
	BurnStatus(int damage=1, int turns=1, bool infinite = false, Unit * target=nullptr);
	void affectUnit() override;
private:
	int damage;
};

class StatBuffStatus : public Status {
public:
	StatBuffStatus(Stat stat, float added_percent = .2f, int turns = 1, bool infinite = false,  Unit * target = nullptr);
	void affectUnit() override;

	virtual float getStatModifier(Stat stat) const override;
private:
	Stat stat;
	float added_percent;
};