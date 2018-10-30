#include "unit.hpp"

// TODO: Design better constructors

// Construct a player unit by default
Unit::Unit() : 
	type(UnitType::PLAYER),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1("PUNCH", this, AttackType::MELEE, new DamageEffect(5), 0),
	attack2("PUNCH", this, AttackType::MELEE, new DamageEffect(5), 0),
	shadow("res/assets/shadow.png")
{

}

Unit::Unit(UnitType type) : 
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1("PUNCH", this, AttackType::MELEE, new DamageEffect(5), 0),
	attack2("PUNCH", this, AttackType::MELEE, new DamageEffect(5), 0),
	shadow("res/assets/shadow.png")
{

}

Unit::Unit(UnitType type, Attack attack1, Attack attack2) :
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1(attack1),
	attack2(attack2) ,
	shadow("res/assets/shadow.png")
{

}

void Unit::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	attack1.setTileSize(width, height);
	attack2.setTileSize(width, height);
	// Set the size of common sprites
	shadow.setSize(width, height / 2);
	// Recalculate the screen position based on the tile size
	calculateScreenPosition();
}

void Unit::setTopMargin(int margin) {
	top_margin = margin;
}

void Unit::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	screenPosition.y() = position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() -= sprite_height - tile_height;
	// Calculate shadow positions
	shadow.setPos(position.x() * tile_width, position.y() * tile_height + tile_height / 2);
}
