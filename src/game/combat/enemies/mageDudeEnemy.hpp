#pragma once

#include "../enemy.hpp"


class MageDudeEnemy : public Enemy {

public:

	MageDudeEnemy();
	~MageDudeEnemy();

protected:

	// Override callback function to customize functionality
	void takeDamageCallback(int damage) override;

private:

	// TODO: Add attacks to class variables

	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	Attack fireball; // Mage dude aoe attack
	Attack dragons_rage; // Mage dude passive buff attack
};