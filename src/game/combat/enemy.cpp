#include "enemy.hpp"

#include "../combat.hpp"

Enemy::Enemy() :
	Unit(UnitType::ENEMY),
	sprite("res/assets/WyvernFighter_Sprite.png")
{
	sprite.setSize(sprite_width, sprite_height);

	// Set the enemy attacks to the same AoE attack
	attack1 = Attack("AoE", this, AttackType::SELF, 0, new DamageEffect(2), 1, false);
	attack2 = Attack("AoE", this, AttackType::SELF, 0, new DamageEffect(2), 1, false);
}

Enemy::~Enemy()
{
}

void Enemy::render()
{

	if (state != UnitState::DEAD) {
		shadow.render();

		sprite.setPos(screenPosition.x(), screenPosition.y());
		sprite.render();
	}

	renderHealth();

}

void Enemy::update(int delta) {
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
	// CHOOSE A RANDOM LOCATION ON THE GRID TO MOVE
	int x = rand() % combat->grid.map_width;
	int y = rand() % combat->grid.map_height;
	// If the movement wasn't successful, skip directly to the attack
	if (!move(*combat, Vec2<int>(x, y))) {
		handleAttack();
	}
}

void Enemy::handleAttack() {
	int key = rand() % 2;
	switch (key) {
	case 0: {
		// do the action here
		attack1.attack(position, *combat);
		state = UnitState::ATTACK;
		startCounter();
	} break;
	case 1: {
		// do the action here
		attack2.attack(position, *combat);
		state = UnitState::ATTACK;
		startCounter();
	} break;
	default: {
		// do nothing
	} break;
	}
}

