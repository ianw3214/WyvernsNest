#pragma once

#include "../enemy.hpp"

#define ENEMY_DEFAULT_MOVE_COUNTER		20
#define ENEMY_DEFAULT_ATTACK_COUNTER	20

class WarriorEnemy : public Enemy {

public:

	WarriorEnemy();
	~WarriorEnemy();

private:

	// TODO: Add attacks to class variables

	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

};