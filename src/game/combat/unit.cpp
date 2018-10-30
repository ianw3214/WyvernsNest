#include "unit.hpp"

// Construct a player unit by default
Unit::Unit() : 
	type(UnitType::PLAYER),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT)
{

}

Unit::Unit(UnitType type) : 
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT)
{

}

void Unit::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	// Recalculate the screen position based on the tile size
	calculateScreenPosition();
}

void Unit::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	screenPosition.y() = position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}
