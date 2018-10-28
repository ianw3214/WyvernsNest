#pragma once

#include "../../engine/core.hpp"

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

	// A flag indicating whether the unit is selected
	bool selected = false;

	// Getter methods
	inline UnitState getState() const { return state; }
	inline UnitType getType() const { return type; }

protected:

	// State variable of the unit
	UnitState state;

private:

	// The type of the unit
	UnitType type;

};