#pragma once

#include "../../engine/core.hpp"

#define DEFAULT_SPRITE_WIDTH 100
#define DEFAULT_SPRITE_HEIGHT 100

// Enumeration of unit types
enum class UnitType {
	PLAYER,
	ENEMY
};

// Enumeration of all possible unit states
enum class UnitState {
	IDLE,
	MOVE
};

class Unit : public Entity {

public:

	Unit();
	Unit(UnitType type);

	// The position of the unit in terms of grid coordinates
	Vec2<int> position;
	// The position of the unit in terms of screen coordinates
	ScreenCoord screenPosition;

	// A flag indicating whether the unit is selected
	bool selected = false;

	// Getter methods
	inline UnitState getState() const { return state; }
	inline UnitType getType() const { return type; }

	// Setter methods
	void setTileSize(int width, int height);

	// The health variables of the unit
	int health;
	int maxHealth;

protected:

	// State variable of the unit
	UnitState state;

	// Helper method to calculate the screen position based on grid position
	void calculateScreenPosition();

	// Helper methods/variables needed for proper sprite rendering
	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;

private:

	// The type of the unit
	UnitType type;

};