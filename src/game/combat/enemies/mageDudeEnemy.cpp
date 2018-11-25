#include "mageDudeEnemy.hpp"

#include "../../combat.hpp"
#include "../../util/attackloader.hpp"

MageDudeEnemy::MageDudeEnemy() :
	// Init class vars
	Enemy(UnitType::ENEMY, "res/assets/enemies/MageDude_Sprite.png"),
	// Attack init
	fireball(Attacks::get("FIREBALL", this)),
	dragons_rage(Attacks::get("DRAGONS RAGE", this))
{
	sprite.setSize(sprite_width, sprite_height);
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
	// Try to find a valid location to move to
	int x_offset;
	int y_offset;
	int tries = 20;
	while (tries > 0) {
		bool hit_up = combat->getUnitAt(position - Vec2<int>(0, 2));
		bool hit_down = combat->getUnitAt(position - Vec2<int>(0, -2));
		bool hit_right = combat->getUnitAt(position - Vec2<int>(2, 0));
		bool hit_left = combat->getUnitAt(position - Vec2<int>(-2, 0));
		bool hit_up_left = combat->getUnitAt(position - Vec2<int>(-1, 1));
		bool hit_up_right = combat->getUnitAt(position - Vec2<int>(1, 1));
		bool hit_down_left = combat->getUnitAt(position - Vec2<int>(-1, -1));
		bool hit_down_right = combat->getUnitAt(position - Vec2<int>(1, -1));

		bool unit_left = combat->getUnitAt(position - Vec2<int>(-1, 0));
		bool unit_right = combat->getUnitAt(position - Vec2<int>(1, 0));
		bool unit_up = combat->getUnitAt(position - Vec2<int>(0, 1));
		bool unit_down = combat->getUnitAt(position - Vec2<int>(0, -1));
		
		// First try to run away, then check if in range
		if (unit_left) {
			x_offset = 1;
			y_offset = 0;
		} else if (unit_right) {
			x_offset = -1;
			y_offset = 0;
		} else if (unit_up) {
			x_offset = 0;
			y_offset = -1;
		} else if (unit_down) {
			x_offset = 0;
			y_offset = 1;
		} else if (hit_up || hit_down || hit_right || hit_left || hit_up_left || hit_up_right || hit_down_left || hit_down_right) {
			x_offset = 0;
			y_offset = 0;
		} else { // No units in sight
			x_offset = rand() % (getMoveSpeed() * 2 + 1) - getMoveSpeed();
			y_offset = getMoveSpeed() - std::abs(x_offset);
		}
		// If we can find a path to the target location, then move to that location
		if (getPath(*combat, position - Vec2<int>(x_offset, y_offset)).size() > 0) {
			break;
		}
		tries--;
	}
	// If the movement fails, then directly execute the attack state
	// NOTE: if movement succeeds, then the handleAttack function doesn't need to be explicitly called
	if (!move(*combat, position - Vec2<int>(x_offset, y_offset))) {
		// Directly handle the attacks if no movement could be done
		handleAttack();
	}
}

void MageDudeEnemy::handleAttack() {
	// A diamond shape AoE
	if (combat->getUnitAt(position - Vec2<int>(2, 0))) {
		fireball.attack(position - Vec2<int>(2, 0), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}

	if (combat->getUnitAt(position - Vec2<int>(0, 2))) {
		fireball.attack(position - Vec2<int>(0, 2), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(0, -2))) {
		fireball.attack(position - Vec2<int>(0, -2), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(-2, 0))) {
		fireball.attack(position - Vec2<int>(-2, 0), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(1, 1))) {
		fireball.attack(position - Vec2<int>(1, 1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(-1, 1))) {
		fireball.attack(position - Vec2<int>(-1, 1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(1, -1))) {
		fireball.attack(position - Vec2<int>(1, -1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}
	if (combat->getUnitAt(position - Vec2<int>(-1, -1))) {
		fireball.attack(position - Vec2<int>(-1, -1), *combat);
		state = UnitState::ATTACK;
		startCounter();
		return;
	}

	state = UnitState::DONE;
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
