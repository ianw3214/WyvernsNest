#pragma once

#include "../enemy.hpp"

#define MAGE_WIDTH_IN_SOURCE			55.f
#define MAGE_HEIGHT_IN_SOURCE			75.f

class MageDudeEnemy : public Enemy {

public:

	MageDudeEnemy();
	~MageDudeEnemy();

protected:

	// Override callback function to customize functionality
	virtual void setTileSizeCallback(int width, int height) override;
	virtual void takeDamageCallback(int damage) override;

private:

	// Helper method to handle the movement portion of an enemy turn
	virtual bool handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	Attack fireball; // Mage dude aoe attack
	Attack dragons_rage; // Mage dude passive buff attack
};