#pragma once

#include "../../engine/core.hpp"

#include <vector>

#include "attack.hpp"
#include "status.hpp"
#include "../unitData.hpp"

#define DEFAULT_SPRITE_WIDTH	300
#define DEFAULT_SPRITE_HEIGHT	300

// The scale of the sprite in relation to the tile size
#define DEFAULT_WIDTH_TO_TILE	1.f
// The scale of the height of the sprite in relation to the width
#define DEFAULT_SPRITE_RATIO	1.5f

class Combat;

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

	// Units should never be default constructed, a type needs to be specified
	Unit(UnitType type);

	// The position of the unit in terms of grid coordinates
	Vec2<int> position;
	// The position of the unit in terms of screen coordinates
	ScreenCoord screenPosition;

	// Render methods
	virtual void renderBottom(Combat * combat);
	// virtual void renderUnit();	<-- USE THE ORIGINAL ENTITY RENDER FUNCTION FOR THIS
	virtual void renderTop(Combat * combat);
	virtual void renderHealth();

	// Getter methods
	UnitState getState() const { return state; }
	UnitType getType() const { return type; }
	// Unit attribute getter methods
	int getStat(Stat stat) const;
	// TODO: (Ian) Remove these functions, made redundant by getStat function
	int getSTR() const;
	int getDEX() const;
	int getINT() const;
	int getCON() const;
	int getMoveSpeed() const { return move_speed; }
	int getMaxHealth() const { return maxHealth; }
	int getSpriteWidth() const { return sprite_width; }
	int getSpriteHeight() const { return sprite_height; };
	int getTileWidth() const { return tile_width; }
	int getTileHeight() const { return tile_height; }

	// Helper method to calculate the screen position based on grid position
	void calculateScreenPosition();

	// Setter methods
	void setTileSize(int width, int height);
	void setTopMargin(int margin);
	void setState(UnitState newState) { state = newState; }

	// The health variables of the unit
	int health;
	int maxHealth;

	// Utility status effect functions
	void addStatus(Status * status);

	// Utility functions common across all units
	void select();
	void deselect();
	void takeDamage(int damage);
	void heal(int health);
	bool move(Combat& combat, Vec2<int> pos);
	void push(int push, ScreenCoord src_pos);

	// Utility references to the combat state to access needed data
	Combat * combat;
protected:

	void setData(UnitData data) { 
		this->data = data;
		loadPropertiesFromUnitData();
	}

	// Variables that contain various useful stats for the unit
	int move_speed;
	bool selected = false;
	void loadPropertiesFromUnitData(bool resetHealth=true);

	// State variable of the unit
	UnitState state;
	// The state counter can be used by setting it to 0 and incrementing every frame
	int state_counter;
	inline void startCounter() { state_counter = 0; }
	inline void incrementCounter() { state_counter++; }
	inline bool compareCounter(int num) const { return state_counter >= num; }

	// Variables to help keep track of unit movement
	std::vector<Vec2<int>> path;
	ScreenCoord moveTarget;
	ScreenCoord moveDiff;
	ScreenCoord moveNext;
	// Helper variables for unit movement
	std::vector<ScreenCoord> getPath(Combat & combat, ScreenCoord to);
	// Pathfinding helper methods
	std::vector<ScreenCoord> heuristic(std::vector<std::vector<ScreenCoord>> * open);
	std::vector<ScreenCoord> getValidNeighbours(ScreenCoord pos, Combat & combat);
	// Helper functions to calculate the screen position and movement of the player
	void calculateScreenPositionMovement();
	void incrementMovement();

	// Helper methods/variables needed for proper sprite rendering
	int sprite_width;
	int sprite_height;
	int tile_width, tile_height;
	int top_margin;
	// Width/height of the unit within the sprite
	int unit_width;
	int unit_height;

	// Virtual functions that units can override to customize functionality
	virtual void setTileSizeCallback(int width, int height);
	virtual void takeDamageCallback(int damage);
	virtual void selectCallback();

	// Common sprites used by most units
	Sprite shadow;

private:

	// The type of the unit
	UnitType type;

	// The unit data of the unit
	UnitData data;
	void generateDefaultUnitData();

	public:
	// The status effects of the unit
	std::vector<Status*> statusList;

};