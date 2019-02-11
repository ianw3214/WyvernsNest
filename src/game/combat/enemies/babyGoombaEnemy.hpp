#pragma once

#include "../enemy.hpp"

#define ENEMY_WIDTH_IN_SOURCE			33.f
#define ENEMY_HEIGHT_IN_SOURCE			33.f


class BabyGoombaEnemy : public Enemy
{
public:
	BabyGoombaEnemy();
	~BabyGoombaEnemy();

protected:

	// Override callback function to customize functionality
	virtual void setTileSizeCallback(int width, int height) override;
	virtual void takeDamageCallback(int damage) override;

private:

	// Helper method to handle the movement portion of an enemy turn
	virtual bool handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	Attack bite; // Bite attack for Goomba
};

