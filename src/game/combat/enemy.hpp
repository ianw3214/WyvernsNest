#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

#include "unit.hpp"
#include "attack.hpp"

#define ENEMY_DEFAULT_MOVE_COUNTER		20
#define ENEMY_DEFAULT_ATTACK_COUNTER	20

class Enemy : public Unit {

public:

	Enemy();
	~Enemy();

	virtual void render();
	virtual void update(int delta);

	// Setter functions
	inline void setCombatReference(Combat * combat) { this->combat = combat; }

	void takeTurn();

protected:
	
	// Helper method to handle the movement portion of an enemy turn
	virtual void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	virtual void handleAttack();

	// Enemy sprite
	Sprite sprite;

	// Hold a reference to the combat state
	Combat * combat;

private:

	// Basic bite attack of enemy
	Attack bite;

};