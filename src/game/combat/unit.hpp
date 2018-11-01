#pragma once

#include "../../engine/core.hpp"

#include "attack.hpp"
#include "../unitData.hpp"

#define DEFAULT_SPRITE_WIDTH 200
#define DEFAULT_SPRITE_HEIGHT 200

// Enumeration of unit types
enum class UnitType {
	PLAYER,
	ENEMY
};

// Enumeration of all possible unit states
enum class UnitState {
	IDLE,
	MOVE,
	ATTACK,
	DONE,
	DEAD
};

class Unit : public Entity {

public:

	Unit();
	Unit(UnitType type);
	Unit(UnitType type, Attack attack1, Attack attack2);

	// The position of the unit in terms of grid coordinates
	Vec2<int> position;
	// The position of the unit in terms of screen coordinates
	ScreenCoord screenPosition;

	// A flag indicating whether the unit is selected
	bool selected = false;

	// Getter methods
	inline UnitState getState() const { return state; }
	inline UnitType getType() const { return type; }
	// Unit attribute getter methods
	inline int getSTR() const { return data.strength; }
	inline int getDEX() const { return data.dexterity; }
	inline int getINT() const { return data.intelligence; }
	inline int getCON() const { return data.constitution; }
	inline int getMoveSpeed() const { return move_speed; }
	inline int getMaxHealth() const { return maxHealth; }


	// Setter methods
	void setTileSize(int width, int height);
	void setTopMargin(int margin);
	void setState(UnitState newState) { state = newState; }


	// The health variables of the unit
	int health;
	int maxHealth;
	void takeDamage(int damage);
	void renderHealth();

	// The attacks of the unit
	Attack attack1;
	Attack attack2;

	// Utility references to the combat state to access needed data
	Combat * combat;

protected:

	// Variables that contain various useful stats for the unit
	int move_speed;
	void loadPropertiesFromUnitData();

	// State variable of the unit
	UnitState state;
	// The state counter can be used by setting it to 0 and incrementing every frame
	int state_counter;
	inline void startCounter() { state_counter = 0; }
	inline void incrementCounter() { state_counter++; }
	inline bool compareCounter(int num) const { return state_counter >= num; }

	// Helper method to calculate the screen position based on grid position
	void calculateScreenPosition();

	// Helper methods/variables needed for proper sprite rendering
	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;
	int top_margin;

	// Common sprites used by most units
	Sprite shadow;

private:

	// The type of the unit
	UnitType type;

	// The unit data of the unit
	UnitData data;
	void generateDefaultUnitData();

};