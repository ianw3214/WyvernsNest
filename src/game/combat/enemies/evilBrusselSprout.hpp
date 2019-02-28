#pragma once

#include "../enemy.hpp"

#define BRUSSEL_WIDTH_IN_SOURCE			33.f
#define BRUSSEL_HEIGHT_IN_SOURCE			33.f


class EvilBrusselSprout : public Enemy
{
public:
	EvilBrusselSprout();
	~EvilBrusselSprout();

protected:
	// Override callback function to customize functionality
	virtual void setTileSizeCallback(int width, int height) override;
	virtual void takeDamageCallback(int damage) override;

private:
	// Helper method to handle the movement portion of an enemy turn
	virtual bool handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();
};

