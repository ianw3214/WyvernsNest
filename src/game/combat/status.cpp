#include "status.hpp"

#include "../combat/unit.hpp"

BurnStatus::BurnStatus(int damage, int turns, Unit * target) : Status(StatusType::BURN, turns, target), damage(damage) {}

void BurnStatus::affectUnit() {
	if (target) target->takeDamage(damage);
}

bool Status::tick() {
	affectUnit();
	return --turns > 0;
}
