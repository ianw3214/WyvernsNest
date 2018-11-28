#include "status.hpp"

#include "../combat/unit.hpp"

bool Status::tick() {
	affectUnit();
	// Returns true if there are still turns left
	return infinite || (--turns > 0);
}

BurnStatus::BurnStatus(int damage, int turns, bool infinite, Unit * target) : 
	Status(StatusType::BURN, turns, infinite, target), damage(damage) {}

void BurnStatus::affectUnit() {
	if (target) target->takeDamage(damage);
}

StatBuffStatus::StatBuffStatus(Stat stat, float added_percent, int turns, bool infinite, Unit * target) :
	Status(StatusType::STAT_BUFF, turns, infinite, target), stat(stat), added_percent(added_percent) {}

void StatBuffStatus::affectUnit() {
	// This does nothing in a stat buff status
}

float StatBuffStatus::getStatModifier(Stat stat) const {
	if (stat == this->stat) return added_percent;
	return 0.0f;
}