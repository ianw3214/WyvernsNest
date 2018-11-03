#include "enemy.hpp"

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

void Enemy::takeTurn(Combat& combat) {
	int key = rand() % 2;
	switch (key) {
	case 0: {
		// do the action here
		attack1.attack(position, combat);
		state = UnitState::ATTACK;
		startCounter();
	} break;
	case 1: {
		// do the action here
		attack2.attack(position, combat);
		state = UnitState::ATTACK;
		startCounter();
	} break;
	default: {
		// do nothing
	} break;
	}
}

