#include "mageDudeEnemy.hpp"

#include "../../combat.hpp"
#include "../../util/attackloader.hpp"

MageDudeEnemy::MageDudeEnemy() :
	// Init class vars
	Enemy(UnitType::ENEMY, "res/assets/enemies/mage.png", 128, 128),
	// Attack init
	fireball(Attacks::get("FIREBALL", this)),
	dragons_rage(Attacks::get("DRAGONS RAGE", this))
{
	sprite.addAnimation(0, 0);			// IDLE
	sprite.addAnimation(1, 1);			// DAMAGE
	sprite.addAnimation(10, 23);		// GENERIC SPELL
	sprite.addAnimation(30, 43);		// FIRE SPELL
	sprite.addAnimation(50, 58);		// DYING
	sprite.addAnimation(58, 58);		// DEAD

	// Randomize enemy stats
	UnitData data;
	data.strength = rand() % 10 + 10;
	data.dexterity = rand() % 10 + 10;
	data.intelligence = rand() % 10 + 10;
	data.constitution = rand() % 10 + 10;
	setData(data);
}

MageDudeEnemy::~MageDudeEnemy() {

}

void MageDudeEnemy::setTileSizeCallback(int width, int height) {
	// TODO: include this in a metadata file
	float width_to_tile = 1.4f;
	float sprite_ratio = 1.8f;
	// Calculate the sprite size based on the width/height
	float width_ratio = static_cast<float>(128 / MAGE_WIDTH_IN_SOURCE);
	sprite_width = static_cast<int>(width_ratio * width_to_tile * width);
	float height_ratio = static_cast<float>(128 / MAGE_HEIGHT_IN_SOURCE);
	sprite_height = static_cast<int>(height_ratio * width * sprite_ratio);
	sprite.setSize(sprite_width, sprite_height);
	calculateScreenPosition();

	// Also set the units height
	unit_height = static_cast<int>(MAGE_HEIGHT_IN_SOURCE * static_cast<float>(sprite_height) / 128.f);
}

void MageDudeEnemy::takeDamageCallback(int damage) {
	if (health >= 0) {
		if (health - damage < 0) {
			sprite.playAnimation(4);
			sprite.queueAnimation(5);
		} else {
			sprite.playAnimation(1, 10);
			sprite.queueAnimation(0);
		}
	}
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
		bool in_range_flag = false;
		bool end_flag = false;
		for (int i = -2; i < 3; i++) {
			for (int j = -2; j < 3; j++) {
				if (i == -2 || i == 2) {
					if (combat->getUnitAt(position - Vec2<int>(i, j))) {
						in_range_flag = true;
						end_flag = true;
						break;
					}
				}
				else {
					if (combat->getUnitAt(position - Vec2<int>(i, -2))) {
						in_range_flag = true;
						end_flag = true;
						break;
					}
					if (combat->getUnitAt(position - Vec2<int>(i, 2))) {
						in_range_flag = true;
						end_flag = true;
						break;
					}
				}
			}
			if (end_flag) break;
		}

		Unit *unit_left = combat->getUnitAt(position - Vec2<int>(-1, 0));
		Unit *unit_right = combat->getUnitAt(position - Vec2<int>(1, 0));
		Unit *unit_up = combat->getUnitAt(position - Vec2<int>(0, 1));
		Unit *unit_down = combat->getUnitAt(position - Vec2<int>(0, -1));
		
		// First try to run away, then check if in range
		if (unit_left && unit_left->getType() == UnitType::PLAYER) {
			x_offset = 1;
			y_offset = 0;
		} else if (unit_right && unit_right->getType() == UnitType::PLAYER) {
			x_offset = -1;
			y_offset = 0;
		} else if (unit_up && unit_up->getType() == UnitType::PLAYER) {
			x_offset = 0;
			y_offset = -1;
		} else if (unit_down && unit_down->getType() == UnitType::PLAYER) {
			x_offset = 0;
			y_offset = 1;
		} else if (in_range_flag) {
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

	if (!move(*combat, position - Vec2<int>(x_offset, y_offset))) {
		// Use base enemies random movement if movement fails
		Enemy::handleMovement();
	}
}

void MageDudeEnemy::handleAttack() {
	// randomly pick an attack
	int coin_flip = rand() % 100;
	Unit *curr_unit;
	bool end_flag = false;
	if (0 <= coin_flip && coin_flip <= 75) { // fireball attack
		for (int i = -2; i < 3; i++) {
			for (int j = -2; j < 3; j++) {
				if (i == -2 || i == 2) {
					curr_unit = combat->getUnitAt(position - Vec2<int>(i, j));
					if (curr_unit && curr_unit->getType() == UnitType::PLAYER) {
						fireball.attack(position - Vec2<int>(i, j), *combat);
						state = UnitState::ATTACK;
						startCounter();
						sprite.playAnimation(3);
						sprite.queueAnimation(0);
						return;
					}
				} else {
					curr_unit = combat->getUnitAt(position - Vec2<int>(i, -2));
					if (curr_unit && curr_unit->getType() == UnitType::PLAYER) {
						fireball.attack(position - Vec2<int>(i, -2), *combat);
						state = UnitState::ATTACK;
						startCounter();
						sprite.playAnimation(3);
						sprite.queueAnimation(0);
						return;
					}
					curr_unit = combat->getUnitAt(position - Vec2<int>(i, 2));
					if (curr_unit && curr_unit->getType() == UnitType::PLAYER) {
						fireball.attack(position - Vec2<int>(i, 2), *combat);
						state = UnitState::ATTACK;
						startCounter();
						sprite.playAnimation(3);
						sprite.queueAnimation(0);
						return;
					}
				}
			}
		}
	} else { // Dragon's rage
		// Find an ally beside and buff 'em
		curr_unit = combat->getUnitAt(position - Vec2<int>(0, 1));
		if (curr_unit && curr_unit->getType() == UnitType::ENEMY) {
			dragons_rage.attack(position - Vec2<int>(0, 1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(2);
			sprite.queueAnimation(0);
			return;
		}
		curr_unit = combat->getUnitAt(position - Vec2<int>(0, -1));
		if (curr_unit && curr_unit->getType() == UnitType::ENEMY) {
			dragons_rage.attack(position - Vec2<int>(0, -1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(2);
			sprite.queueAnimation(0);
			return;
		}
		curr_unit = combat->getUnitAt(position - Vec2<int>(1, 0));
		if (curr_unit && curr_unit->getType() == UnitType::ENEMY) {
			dragons_rage.attack(position - Vec2<int>(1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(2);
			sprite.queueAnimation(0);
			return;
		}
		curr_unit = combat->getUnitAt(position - Vec2<int>(-1, 0));
		if (curr_unit && curr_unit->getType() == UnitType::ENEMY) {
			dragons_rage.attack(position - Vec2<int>(-1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(2);
			sprite.queueAnimation(0);
			return;
		}
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
