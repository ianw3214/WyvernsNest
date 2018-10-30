#include "attack.hpp"
#include "../combat.hpp"

Attack::Attack(std::string name, 
	Unit * source, 
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

void Attack::attack(ScreenCoord pos, Combat& combat) {
	effect->attack(pos, combat);
	// TODO: figure out how to apply the attack effect to the surrounding aoe
}

void Attack::showValidGrid(ScreenCoord playerPos, ScreenCoord grid) {

}

bool Attack::isValid(ScreenCoord pos) {
	// TODO: figure out how to determine if coordinate is valid based on attack range
	return false;
}

#include <iostream>
void DamageEffect::attack(ScreenCoord pos, Combat & combat) {
	Unit * unit = combat.getUnitAt(pos);
	std::cout << "DO HELLO" << pos.x() << ", " << pos.y() << std::endl;
	if (unit) {
		std::cout << "DO DAMAGE" << std::endl;
		unit->health -= damage;
		// Do something if the unit dies
	}
}
