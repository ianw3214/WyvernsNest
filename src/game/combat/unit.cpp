#include "unit.hpp"

// Construct a player unit by default
Unit::Unit() : 
	type(UnitType::PLAYER),
	state(UnitState::IDLE)
{

}

Unit::Unit(UnitType type) : 
	type(type),
	state(UnitState::IDLE)
{

}
