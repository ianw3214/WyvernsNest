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
	aoe(aoe),
	validSprite("res/test8.png"),
	targetValidSprite("res/test6.png"),
	targetInvalidSprite("res/test7.png")
{
	validSprite.setSize(213, 180);
	targetValidSprite.setSize(213, 180);
	targetInvalidSprite.setSize(213, 180);
}

void Attack::attack(ScreenCoord pos, Combat& combat) {
	effect->attack(pos, combat);
	// TODO: figure out how to apply the attack effect to the surrounding aoe
}

// Display the valid attack tiles on the grid
void Attack::renderValidGrid() {
	switch (type) {
	case AttackType::MELEE: {
		int xScale = 213;
		int yScale = 180;
		validSprite.setPos((source->position.x() - 1) * xScale, source->position.y() * yScale);
		validSprite.render();
		validSprite.setPos(source->position.x() * xScale, (source->position.y() - 1) * yScale);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * xScale, source->position.y() * yScale);
		validSprite.render();
		validSprite.setPos(source->position.x() * xScale, (source->position.y() + 1) * yScale);
		validSprite.render();
	} break;
	case AttackType::PLUS: {
		// TODO: More descriptive name than plus
		// TODO: Display the sprites correctly
		// TODO: Add a range property and figure out valid grid based off of range
	} break;
	}
	// TODO: get the mouse position somehow to render whether the target position is valid
	/*
	if (isValid(pos)) {
		//TODO values here are temporary
		targetValidprite.setPos(pos.x() * 213, pos.y() * 180);
		targetValidprite.setSize(213, 180);
		targetValidprite.render();
	}
	else {
		//TODO values here are temporary
		targetInvalidSprite.setPos(pos.x() * 213, pos.y() * 180);
		targetInvalidSprite.setSize(213, 180);
		targetInvalidSprite.render();
	}
	*/
}

bool Attack::isValid(ScreenCoord pos) {
	switch (type) {
	case AttackType::MELEE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		/*
		if ((pos.x() == source->position.x() + 1 || pos.x() == source->position.x() - 1) && pos.y() == source->position.y()) {
			return true;
		}
		else if ((pos.y() == source->position.y() + 1 || pos.y() == source->position.y() - 1) && pos.x() == source->position.x()) {
			return true;
		}
		*/
	} break;
	case AttackType::PLUS: {
		// TODO: More descriptive name than plus
		// TODO: Calculate whether the pos is valid based off a range property
	}
	}
	return false;
}

void DamageEffect::attack(ScreenCoord pos, Combat & combat) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		unit->health -= damage;
		// Do something if the unit dies
	}
}
