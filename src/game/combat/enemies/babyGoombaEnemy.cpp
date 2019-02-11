#include "babyGoombaEnemy.hpp"

#include "../../combat.hpp"
#include "../../util/attackloader.hpp"

BabyGoombaEnemy::BabyGoombaEnemy() :
	Enemy(UnitType::ENEMY, "res/assets/enemies/babygoomba.png", 64, 64),
	bite(Attacks::get("PUNCH", this))
{
	sprite.setSize(sprite_width, sprite_height);
	sprite.setSourceSize(64, 64);

	sprite.addAnimation(0, 0);			// IDLE
	sprite.addAnimation(1, 1);			// DAMAGE
	sprite.addAnimation(10, 17);		// DYING
	sprite.addAnimation(18, 18);		// DEAD

	// Randomize enemy stats
	UnitData data;
	data.strength = rand() % 10 + 1;
	data.dexterity = rand() % 10 + 1;
	data.intelligence = rand() % 10 + 1;
	data.constitution = rand() % 10 + 1;
	setData(data);
}


BabyGoombaEnemy::~BabyGoombaEnemy()
{
}

void BabyGoombaEnemy::setTileSizeCallback(int width, int height) 
{
	// Calculate the sprite size based on the width/height
	float width_ratio = static_cast<float>(64 / ENEMY_WIDTH_IN_SOURCE);
	sprite_width = static_cast<int>(width_ratio * DEFAULT_WIDTH_TO_TILE * width);
	float height_ratio = static_cast<float>(64 / ENEMY_HEIGHT_IN_SOURCE);
	// TODO: include this in file metadata as well
	float sprite_ratio = 1.f;
	sprite_height = static_cast<int>(height_ratio * width * sprite_ratio);
	sprite.setSize(sprite_width, sprite_height);
	calculateScreenPosition();

	// Also set the units height
	unit_height = static_cast<int>(ENEMY_HEIGHT_IN_SOURCE * static_cast<float>(sprite_height) / 64.f);
}

void BabyGoombaEnemy::takeDamageCallback(int damage) 
{
	if (health >= 0) {
		if (health - damage < 0) {
			sprite.playAnimation(2);
			sprite.queueAnimation(3);
		}
		else {
			sprite.playAnimation(1, 10);
			sprite.queueAnimation(0);
		}
	}
}

bool BabyGoombaEnemy::handleMovement() 
{
	// Default enemy random movement
	bool success = Enemy::handleMovement();
	if (!success) {
		// If even the base random movement fails move to attacks
		handleAttack();
		return false;
	}
	return true;
}

void BabyGoombaEnemy::handleAttack() 
{
	Unit *targ_unit;

	targ_unit = combat->getUnitAt(position - Vec2<int>(1, 0));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			// do the action here
			bite.attack(position - Vec2<int>(1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			return;
		}
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, 1));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			// do the action here
			bite.attack(position - Vec2<int>(0, 1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			return;
		}
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(-1, 0));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			// do the action here
			bite.attack(position - Vec2<int>(-1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			return;
		}
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, -1));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			// do the action here
			bite.attack(position - Vec2<int>(0, -1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			return;
		}
	}
	// If no attacks could be done, set the unit to be at done state
	state = UnitState::DONE;
}