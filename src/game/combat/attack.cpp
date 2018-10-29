#include "attack.hpp"

Attack::Attack(std::string name, 
	ScreenCoord source, 
	AttackType type,
	AttackEffect * effect, 
	AttackAoE aoe) 
	:
	name(name),
	source(source),
	type(type),
	effect(effect),
	aoe(aoe)
{

}

void Attack::attack(ScreenCoord pos, std::vector<Unit*>& units) {
	effect->attack(pos, units);
	// TODO: figure out how to apply the attack effect to the surrounding aoe
}

void Attack::showValidGrid(ScreenCoord playerPos, ScreenCoord grid) {

}

bool Attack::isValid(ScreenCoord pos) {
	// TODO: figure out how to determine if coordinate is valid based on attack range
	return false;
}
