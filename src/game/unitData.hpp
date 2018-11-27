/**
  *		Data structure to hold unit data
  *			- On top of basic attributes/stats, units have a list of traits
  *			- Each trait can have 2 stat bonuses, which may be positive/negative
  *
  *		MISSING: 
  *			- Skill tree storing for units
  *			- Experience points for units
  *			- Chosen unit actions ( for combat )
  */
#pragma once

#include <string>
#include <vector>
#include <utility> 

// An enumeration representing the stat types
enum class Stat { STR, DEX, INT, CON };

// A structure representing character traits
struct Trait {
	// The name of the trait
	std::string name;
	// A description of the trait
	std::string description;
	// Stat bonuses of the trait
	std::pair<Stat, int> primary;
	std::pair<Stat, int> secondary;
};

// A structure holding all data of a character
struct UnitData {

	// The name of the unit
	std::string name;

	// The stats of the unit
	int strength;
	int dexterity;
	int intelligence;
	int constitution;

	// The EXP of the unit
	int level;
	int experience;

	// Traits of the unit
	std::vector<Trait> traits;

	// Skill tree of the unit
	std::vector<int> skillTree;

	// Selected attacks
	std::string attack1;
	std::string attack2;
	std::string attack3;
	std::string attack4;

};