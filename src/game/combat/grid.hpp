#pragma once

#include "../../engine/core.hpp"
#include "Windows.h"
#include "../../math/vec.hpp"
#include <cmath> 

#define TILE_WIDTH 100
#define TILE_HEIGHT 100

#define DEFAULT_MAP_WIDTH 4
#define DEFAULT_MAP_HEIGHT 3

#define DEFAULT_TILEMAP { 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0 }
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

	int map_width;
	int map_height;

	int spacing = 10;


	int mouseX;
	int mouseY;

	ScreenCoord mousePos;

private:

	Sprite tile1;
	Sprite tile2;
	Sprite selected;

};