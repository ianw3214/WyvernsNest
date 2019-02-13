#include "basicWarriorEnemy.hpp"

#include "../../combat.hpp"

#include "../../util/attackloader.hpp"
#include "../player.hpp"

#include <fstream>
#include <iostream>

WarriorEnemy::WarriorEnemy() : 
	// Init class vars
	Enemy(UnitType::ENEMY, "res/assets/enemies/warrior.png", 196, 196),
	// Attack init
	hit(Attacks::get("HIT", this)),
	block(Attacks::get("BLOCK", this))
{
	sprite.addAnimation(0, 0);			// IDLE
	sprite.addAnimation(1, 1);			// DAMAGE
	sprite.addAnimation(2, 2);			// DEAD
	sprite.addAnimation(10, 21);		// ATTACK

	int STR = 0;
	int DEX = 0;
	int INT = 0;
	int CON = 0;
	// Load enemy data from file
	const std::string filePath = "res/data/enemies/warrior.json";
	std::ifstream file(filePath);
	if (file.is_open()) {
		json data;
		file >> data;

		json stats = data["BASIC WARRIOR"];
		STR = stats["STR"];
		DEX = stats["DEX"];
		INT = stats["INT"];
		CON = stats["CON"];
	}
	else {
		std::cerr << "ERROR: load BASIC WARRIOR. All stats default to 0.\n";
	}

	UnitData udata;
	udata.strength = STR;
	udata.dexterity = DEX;
	udata.intelligence = INT;
	udata.constitution = CON;
	setData(udata);
}

WarriorEnemy::~WarriorEnemy() {

}

void WarriorEnemy::setTileSizeCallback(int width, int height) {
	// TODO: include this in a metadata file
	float width_to_tile = 1.6f;
	float sprite_ratio = 1.6f;
	// Calculate the sprite size based on the width/height
	float width_ratio = static_cast<float>(196 / WARRIOR_WIDTH_IN_SOURCE);
	sprite_width = static_cast<int>(width_ratio * width_to_tile * width);
	float height_ratio = static_cast<float>(196 / WARRIOR_HEIGHT_IN_SOURCE);
	sprite_height = static_cast<int>(height_ratio * width * sprite_ratio);
	sprite.setSize(sprite_width, sprite_height);
	calculateScreenPosition();

	// Also set the units height
	unit_height = static_cast<int>(WARRIOR_HEIGHT_IN_SOURCE * static_cast<float>(sprite_height) / 196.f);
}

void WarriorEnemy::takeDamageCallback(int damage) {
	if (health >= 0) {
		if (health - damage < 0) {
			sprite.playAnimation(2);
		} else {
			sprite.playAnimation(1, 10);
			sprite.queueAnimation(0);
		}
	}
}

// Calculates euclidian distance between 2 positions
int distance(Vec2<int> p1, Vec2<int> p2){
	return static_cast<int>(sqrt(pow(p1[0]-p2[0],2)-pow(p1[1]-p2[1],2)));
}	

bool WarriorEnemy::handleMovement() {

	/*
		- The warrior enemy should simply move towards the player every turn

		- If the warrior enemy is already beside a player, then we should go
			directly to the handle attack state
	*/


	int moveSpeed = 2;
	int min_distance = INT_MAX;
	int max_grid = 4;

	//target the closest player
	Player * target_player = NULL;
	std::vector<Player*> players = combat->getPlayers();
	for (Player *& p : players){	
		if (p->getType() == UnitType::PLAYER) {
			if (!(p->getState() == UnitState::DEAD)) {
				if (distance(position, p->position) < min_distance) {
					target_player = p;
					min_distance = distance(position, p->position);
				}
			}
		}
	}

	Vec2<int> target_position;

	// Set target_position one of these: left,right,top,bottom of target_player
	// According to which one is closer
	min_distance=INT_MAX;
	Vec2<int> temp_target =target_position;

	temp_target = {target_player->position[0]-1,target_player->position[1]};
	if(combat->isPosEmpty(temp_target) && distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0]+1,target_player->position[1]};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0],target_player->position[1]-1};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}
	temp_target = {target_player->position[0],target_player->position[1]+1};
	if(combat->isPosEmpty(temp_target) &&distance(temp_target,position)<min_distance){
		min_distance=distance(temp_target,position);
		target_position=temp_target;
	}

	// Set the actual position to one in range in case the target_position is out of range
	Vec2<int> position_within_range=target_position;
	if(getPath(*combat, target_position).size() > static_cast<unsigned int>(moveSpeed+1)){
		position_within_range = {getPath(*combat, target_position)[moveSpeed][0],getPath(*combat, target_position)[moveSpeed][1]};
	}	

	if (!move(*combat,position_within_range)) {
		// Use base enemies random movement if movement fails
		int success = Enemy::handleMovement();
		if (!success) {
			// If even the base random movement fails move to attacks
			handleAttack();
			return false;
		}
    }
	return true;
}

void WarriorEnemy::handleAttack() {
	// TODO: Implement BLOCK

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
	// If there is a player adjacent to the enemy, attack the player
	Unit *targ_unit;

	targ_unit = combat->getUnitAt(position - Vec2<int>(1, 0));
    if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			hit.attack(position - Vec2<int>(1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(3);
			sprite.queueAnimation(0);
			return;
		}
    }
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, 1));
    if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			hit.attack(position - Vec2<int>(0, 1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(3);
			sprite.queueAnimation(0);
			return;
		}
    }
	targ_unit = combat->getUnitAt(position - Vec2<int>(-1, 0));
    if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			hit.attack(position - Vec2<int>(-1, 0), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(3);
			sprite.queueAnimation(0);
			return;
		}
    }
	targ_unit = combat->getUnitAt(position - Vec2<int>(0, -1));
    if (targ_unit && targ_unit->getType() == UnitType::PLAYER) {
		if (!(targ_unit->getState() == UnitState::DEAD)) {
			hit.attack(position - Vec2<int>(0, -1), *combat);
			state = UnitState::ATTACK;
			startCounter();
			sprite.playAnimation(3);
			sprite.queueAnimation(0);
			return;
		}
    }
    // If no attacks could be done, set the unit to be at done state
	state = UnitState::DONE;
}
