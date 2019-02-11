#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "../../engine/animatedSprite.hpp"

#include "unit.hpp"
#include "attack.hpp"

// TODO: Put this data in a file w/ metadata
#define ENEMY_DEFAULT_MOVE_COUNTER		12
#define ENEMY_DEFAULT_ATTACK_COUNTER	12

class Enemy : public Unit {

public:

	Enemy(UnitType type, const std::string& spritePath, int src_w = 96, int src_h = 96);
	~Enemy();

	virtual void render() override;
	virtual void update(int delta);

	void takeTurn();

protected:
	
	// Helper method to handle the movement portion of an enemy turn
	virtual bool handleMovement();
	virtual void handleAttack();

	// Enemy sprite
	AnimatedSprite sprite;

};