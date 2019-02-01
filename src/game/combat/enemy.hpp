#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "../../engine/animatedSprite.hpp"

#include "unit.hpp"
#include "attack.hpp"

// TODO: Put this data in a file w/ metadata
#define ENEMY_WIDTH_IN_SOURCE			33.f
#define ENEMY_HEIGHT_IN_SOURCE			33.f

#define ENEMY_DEFAULT_MOVE_COUNTER		12
#define ENEMY_DEFAULT_ATTACK_COUNTER	12

class Enemy : public Unit {

public:

	Enemy();
	Enemy(UnitType type, const std::string& spritePath, int src_w = 96, int src_h = 96);
	~Enemy();

	virtual void render() override;
	virtual void update(int delta);

	void takeTurn();

protected:
	
	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	// Override callback function to customize functionality
	virtual void setTileSizeCallback(int width, int height) override;
	virtual void takeDamageCallback(int damage) override;

	// Enemy sprite
	AnimatedSprite sprite;

private:

	// Basic bite attack of enemy
	Attack bite;
 
};