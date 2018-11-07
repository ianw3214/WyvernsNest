#include "unit.hpp"

#include "../util/attackloader.hpp"

// TODO: Design better constructors

// Construct a player unit by default
Unit::Unit() :
	type(UnitType::PLAYER),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1(Attacks::get("PUNCH", this)),
	attack2(Attacks::get("RANGED", this)),
	shadow("res/assets/shadow.png")
{
  generateDefaultUnitData();
	loadPropertiesFromUnitData();
}

Unit::Unit(UnitType type) :
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1(Attacks::get("PUNCH", this)),
	attack2(Attacks::get("RANGED", this)),
	shadow("res/assets/shadow.png")
{
  generateDefaultUnitData();
	loadPropertiesFromUnitData();
}

Unit::Unit(UnitType type, Attack attack1, Attack attack2) :
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	attack1(attack1),
	attack2(attack2),
	shadow("res/assets/shadow.png")
{
  generateDefaultUnitData();
	loadPropertiesFromUnitData();
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

// Not implemented in base unit, should be implemented in specialized classes
void Unit::takeDamageCallback(int damage) {}
void Unit::selectCallback() {}

void Unit::generateDefaultUnitData() {
	// Default name
	data.name = "BOB";
	// Default stats
	data.strength = 10;
	data.dexterity = 10;
	data.intelligence = 10;
	data.constitution = 100;
	// Default traits -> NOT YET IMPLEMENTED
}

void Unit::select() {
	selected = true;
	selectCallback();
}

void Unit::deselect() {
	selected = false;
}

void Unit::takeDamage(int damage) {
	health -= damage;
	if (health <= 0) {
		health = 0;
		state = UnitState::DEAD;
	}
	// TODO: use different logic for healing
	else if (health > maxHealth) {
		health = maxHealth;
	}
	// Call the virtualized callback function for subclasses to customize
	takeDamageCallback(damage);
}

void Unit::renderHealth() {
	// ScreenCoord pos = screenPosition + ScreenCoord((tile_width - sprite_width) / 2, (tile_height - sprite_height) / 2);
	ScreenCoord pos = screenPosition;
	pos.x() += (sprite_width - tile_width) / 2;
	int healthBarWidth = tile_width;
	int tick = lerp(0, healthBarWidth, static_cast<float>(health) / static_cast<float>(maxHealth));
	// TODO: Use rectangle rendering (implement in engine)
	for (int i = 0; i < 10; ++i) {
		Core::Renderer::drawLine(pos + ScreenCoord(0, i), pos + ScreenCoord(tick, i), Colour(0.0f, 1.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(tick, i), pos + ScreenCoord(healthBarWidth, i), Colour(1.0f, 0.0f, 0.0f));
	}
	Core::Renderer::drawLine(pos + ScreenCoord(0, 0), pos + ScreenCoord(healthBarWidth, 0), Colour(0.0f, 0.0f, 0.0f));
	Core::Renderer::drawLine(pos + ScreenCoord(0, 0), pos + ScreenCoord(0, 10), Colour(0.0f, 0.0f, 0.0f));
	Core::Renderer::drawLine(pos + ScreenCoord(healthBarWidth, 0), pos + ScreenCoord(healthBarWidth, 10), Colour(0.0f, 0.0f, 0.0f));
	Core::Renderer::drawLine(pos + ScreenCoord(0, 10), pos + ScreenCoord(healthBarWidth, 10), Colour(0.0f, 0.0f, 0.0f));
}

void Unit::loadPropertiesFromUnitData() {
	// The health of the unit depends on it's constitution
	health = data.constitution;
	maxHealth = data.constitution;
	// The movement speed in terms of grid units of the unit
	move_speed = data.dexterity / 5 + 1;
}
