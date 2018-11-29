#pragma once

#include "../enemy.hpp"

class WarriorEnemy : public Enemy {

public:

	WarriorEnemy();
	~WarriorEnemy();

protected:

	// Override callback function to customize functionality
	void takeDamageCallback(int damage) override;

private:

	// TODO: Add attacks to class variables 

	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	Attack hit;
	Attack block;

};