#include "status.hpp"

#include "../combat/unit.hpp"

bool Status::tick() {
	affectUnit();
	// Returns true if there are still turns left
	return --turns > 0;
}

BurnStatus::BurnStatus(int damage, int turns, Unit * target) : Status(StatusType::BURN, turns, target), damage(damage) {}

void BurnStatus::affectUnit() {
	if (target) target->takeDamage(damage);
}
