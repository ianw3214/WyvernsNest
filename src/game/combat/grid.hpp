#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "player.hpp"
#include "enemy.hpp"

#define SOURCE_TILE_WIDTH 32
#define SOURCE_TILE_HEIGHT 32
#define SOURCE_WIDTH 6
#define SOURCE_HEIGHT 4
#define INDEX_TO_X(i) (i % SOURCE_WIDTH)
#define INDEX_TO_Y(i) (SOURCE_HEIGHT - (i / SOURCE_WIDTH) - 1)

// TODO: load tilemap from a file
// TODO: store tiles that are 'blocked' so units can't move to that position
#define DEFAULT_MAP_WIDTH 8
#define DEFAULT_MAP_HEIGHT 7
#define DEFAULT_TILEMAP { 23, 23, 23, 23, 23, 23, 23, 23, \
		8,  13, 13, 13, 13, 13, 13, 11, \
		8,  13,  9, 13, 18, 13, 13, 11, \
		8,  13,  9, 18, 13, 15, 16, 17, \
		8,  13,  9, 13, 11, 21, 21, 21, \
		8,  13,  9, 13, 11, 21, 21, 21, \
		14, 16, 15, 16, 17, 21, 21, 21  }
#define TILE_INDEX(x, y) (y * map_width + x)

class Grid {	

public:

	Grid();
	~Grid();

	void render();
	void update();

	// Helper functions
	ScreenCoord getMouseToGrid();
	bool isMousePosValid();
	bool isPosEmpty(Vec2<int> pos) const;

	bool isPosValid(Vec2<int> pos) const;

	// The actual tilemap data of the grid for rendering purposes
	std::vector<int> tilemap;
	std::vector<bool> collisionmap;

	// Grid properties
	int tile_width;
	int tile_height;
	int map_width;
	int map_height;
	int top_margin;

	// Mouse properties
	int mouseX;
	int mouseY;
	// MousePos is the grid coordinates of the mouse
	ScreenCoord mousePos;

	// Debugging methods
	bool renderOutline;

private:

	// Grid sprites
	Sprite tilesheet;

};