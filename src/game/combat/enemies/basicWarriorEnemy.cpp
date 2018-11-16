#include "basicWarriorEnemy.hpp"

WarriorEnemy::WarriorEnemy() {
	// TODO: initialize warrior enemy variables
}

WarriorEnemy::~WarriorEnemy() {

}

void WarriorEnemy::handleMovement() {
	// TODO: Implement warrior enemy movement AI

	/*
		- The warrior enemy should simply move towards the player every turn

		- If the warrior enemy is already beside a player, then we should go
			directly to the handle attack state
	*/
}

void WarriorEnemy::handleAttack() {
	// TODO: Implement warrior enemy attack AI

	/*
		- The warrior enemy has 2 basic attacks: HIT and BLOCK

		- The HIT attack is just a basic melee attack that hits players
		
		- The BLOCK attack should apply a passive effect on the enemy that
			halves incoming damage
		- Passive effects are not implemented in the game yet, so this can
			not be implemneted right now

		- The choosing of which attack to use can be random OR based on player
			position or something. Creative freedom is up to you!
	*/
}
