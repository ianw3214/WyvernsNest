#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"

#define TILE_WIDTH 100
#define TILE_HEIGHT 100

// TODO: load tilemap from a file
#define DEFAULT_MAP_WIDTH 6
#define DEFAULT_MAP_HEIGHT 4
#define DEFAULT_TILEMAP { 1, 1, 0, 0, 0, 0, \
		1, 1, 1, 1, 1, 1, \
		0, 1, 0, 0, 1, 0, \
		1, 0, 0, 1, 1, 0 }
#define TILE_INDEX(x, y) (y * map_width + x)

class Grid {	

public:

	Grid();
	~Grid();

	void render();
	void update();
	ScreenCoord getMouseToGrid();

	bool isMousePosValid();

	std::vector<int> tilemap;

	int tile_width;
	int tile_height;
	int map_width;
	int map_height;

	int mouseX;
	int mouseY;

	ScreenCoord mousePos;

private:

	Sprite tile1;
	Sprite tile2;
	Sprite selected;

};