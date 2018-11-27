#include "mageDudeEnemy.hpp"

MageDudeEnemy::MageDudeEnemy() {
	// TODO: initialize mage dude enemy variables
}

MageDudeEnemy::~MageDudeEnemy() {

}

void MageDudeEnemy::handleMovement() {
	// TODO: Implement mage dude enemy movement AI

	/*
		- The mage dude enemy should walk away from the player in order to stay safe

		- The mage dude enemy should also try to stay in range of a unit for its
			attacks, either for an attack on a player or buff on an ally
	*/
}

void MageDudeEnemy::handleAttack() {
	// TODO: Implement mage dude enemy attack AI

	/*
		- The mage dude enemy has 2 basic attacks: FIREBALL and DRAGONS RAGE

		- The FIREBALL attack is a ranged AoE attack
			- Exact stats are up for tweaking, explore your creative side for now

		- The DRAOGNS RAGE attack should apply a passive effect on the target ally
		- Passive effects are not implemented in the game yet, so this can
			not be implemneted right now

		- The choosing of which attack to use can be random OR based on player
			position or something. Creative freedom is up to you!
	*/
}
