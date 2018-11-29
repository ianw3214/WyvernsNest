#include "enemy.hpp"

#include "../combat.hpp"
#include "../util/attackloader.hpp"

Enemy::Enemy() :
	Unit(UnitType::ENEMY),
	sprite("res/assets/enemies/babygoomba.png", 64, 64),
	bite(Attacks::get("PUNCH", this))
{
	sprite.setSize(sprite_width, sprite_height);
	sprite.setSourceSize(64, 64);
	
	sprite.addAnimation(0, 0);			// IDLE
	sprite.addAnimation(1, 1);			// DAMAGE
	sprite.addAnimation(10, 17);		// DYING
	sprite.addAnimation(18, 18);		// DISAPPEARED

	// Randomize enemy stats
	UnitData data;
	data.strength = rand() % 10 + 1;
	data.dexterity = rand() % 10 + 1;
	data.intelligence = rand() % 10 + 1;
	data.constitution = rand() % 10 + 1;
	setData(data);
}

Enemy::Enemy(UnitType type, const std::string& spritePath) :
	Unit(type),
	sprite(spritePath, 64, 64),
	bite(Attacks::get("PUNCH", this))
{
	sprite.setSize(sprite_width, sprite_height);
}

Enemy::~Enemy()
{
}

void Enemy::render()
{
	sprite.setPos(screenPosition.x(), screenPosition.y());
	sprite.render();

	renderHealth();
}

void Enemy::update(int delta) {
	Unit::update(delta);
	switch (state) {
	case UnitState::IDLE: {
		// Do nothing when idling
	} break;
	case UnitState::MOVE: {
		// Move the enemy towards its destination
		if (compareCounter(ENEMY_DEFAULT_MOVE_COUNTER)) {
			startCounter();
			incrementMovement();
			// If the enemy reaches the target destination, stop moving it
			if (position.x() == moveTarget.x() && position.y() == moveTarget.y()) {
				state = UnitState::IDLE;
				position = moveTarget;
				calculateScreenPosition();
				// Assume that when the enemy only moves on its own turn
				// THUS, handle enemy attack when it finishes moving
				handleAttack();
			}
		} else {
			incrementCounter();
			calculateScreenPositionMovement();
		}
	} break;
	case UnitState::ATTACK: {
		if (compareCounter(ENEMY_DEFAULT_ATTACK_COUNTER)) {
			state = UnitState::DONE;
		} else {
			incrementCounter();
		}
	} break;
	default: {
		// do nothing
	} break;
	}
}

void Enemy::takeTurn() {
	// First, handle enemy movement once it enters its turn
	handleMovement();
}

void Enemy::handleMovement() {
	// Try to move to a random valid location
	int x_offset;
	int y_offset;
	int tries = 10;
	while (tries > 0) {
		x_offset = rand() % (getMoveSpeed() * 2 + 1) - getMoveSpeed();
		y_offset = rand() % (std::abs(getMoveSpeed() - std::abs(x_offset)) * 2 + 1) - std::abs(getMoveSpeed() - std::abs(x_offset));
		if (getPath(*combat, position - Vec2<int>(x_offset, y_offset)).size() > 0) {
			break;
		}
		tries--;
	}
	if (!move(*combat, position - Vec2<int>(x_offset, y_offset))) {
		// Directly handle the attacks if no movement could be done
		handleAttack();
	}
}

void Enemy::handleAttack() {
	Unit *targ_unit;
	
	targ_unit = combat->getUnitAt(position - Vec2<int>(1, 0));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		// do the action here
		bite.attack(position - Vec2<int>(1, 0), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, 1));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		// do the action here
		bite.attack(position - Vec2<int>(0, 1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(-1, 0));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		// do the action here
		bite.attack(position - Vec2<int>(-1, 0), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, -1));
	if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		// do the action here
		bite.attack(position - Vec2<int>(0, -1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	// If no attacks could be done, set the unit to be at done state
	state = UnitState::DONE;
}

void Enemy::takeDamageCallback(int damage) {
	if (health >= 0) {
		if (health - damage < 0) {
			sprite.playAnimation(2);
			sprite.queueAnimation(3);
		} else {
			sprite.playAnimation(1, 10);
			sprite.queueAnimation(0);
		}
	}
}

