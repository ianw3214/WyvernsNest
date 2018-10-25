#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "player.hpp"
#include "enemy.hpp"


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

	Grid(std::vector<Player>* players, std::vector<Enemy>* enemies);


	void render();
	void update();
	ScreenCoord getMouseToGrid();

	void getPlayerPositions();

	bool isMousePosValid();

	std::vector<int> tilemap;

	int tile_width;
	int tile_height;
	int map_width;
	int map_height;

	int mouseX;
	int mouseY;

	ScreenCoord mousePos;

	std::vector<Player>* players;

	std::vector<Enemy>* enemies;

private:

	Sprite tile1;
	Sprite tile2;
	Sprite selected;

};