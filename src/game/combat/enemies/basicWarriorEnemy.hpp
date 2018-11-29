#pragma once

#include "../enemy.hpp"

// TODO: Put this data in a file w/ metadata
#define WARRIOR_WIDTH_IN_SOURCE				66.f
#define WARRIOR_HEIGHT_IN_SOURCE			72.f

class WarriorEnemy : public Enemy {

public:

	WarriorEnemy();
	~WarriorEnemy();

protected:

	// Override callback function to customize functionality
	virtual void setTileSizeCallback(int width, int height) override;
	virtual void takeDamageCallback(int damage) override;

private:

	// TODO: Add attacks to class variables 

	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	Attack hit;
	Attack block;

};