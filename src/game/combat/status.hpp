#pragma once

class Unit;

enum class StatusType {
	BURN
};

class Status {
public:
	Status(StatusType type, int turns, Unit * target) : type(type), turns(turns), target(target) {}
	bool tick();

	void setTarget(Unit * unit) { target = unit; }
protected:
	virtual void affectUnit() = 0;
	Unit * target;
private:
	StatusType type;
	int turns;
};

class BurnStatus : public Status {
public:
	BurnStatus(int damage=1, int turns=1, Unit * target=nullptr);
	void affectUnit() override;
private:
	int damage;
};