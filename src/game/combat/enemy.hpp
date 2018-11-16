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

	void render();
	void update(int delta);

	// Setter functions
	inline void setCombatReference(Combat * combat) { this->combat = combat; }

	void takeTurn();

private:

	// Basic bite attack of enemy
	Attack bite;
	
	// Helper method to handle the movement portion of an enemy turn
	void handleMovement();
	// Helper method to handle the attack portion of an enemy turn
	void handleAttack();

	Sprite sprite;

	// Hold a reference to the combat state
	Combat * combat;

};