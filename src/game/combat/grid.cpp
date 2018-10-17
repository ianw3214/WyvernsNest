#include "grid.hpp"

Grid::Grid() :
	tilemap(DEFAULT_TILEMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tile1("res/test.png"), 
	tile2("res/test2.png"), 
	selected("res/test3.png") 
{
	// Initialize the tile sprites to the tile width/height
	tile1.setSize(TILE_WIDTH, TILE_HEIGHT);
	tile2.setSize(TILE_WIDTH, TILE_HEIGHT);
	selected.setSize(TILE_WIDTH, TILE_HEIGHT);
}

Grid::~Grid() {

}

void Grid::update()
{
	//mouseOn = GetCursorPos(&mouse);
	SDL_GetMouseState(&mouseX, &mouseY);
	mousePos = getMouseToGrid();
}


ScreenCoord Grid::getMouseToGrid()
{
	int x = static_cast<int>(floor(mouseX / 110));
	int y = static_cast<int>(floor(mouseY / 110));

	return ScreenCoord(x,y);
}

bool Grid::isMousePosValid()
{
	return mousePos.x() < map_width && mousePos.y() < map_height;
}


void Grid::render()
{
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			if (tilemap[TILE_INDEX(x, y)] == 1) {

				tile1.setPos(100 * x + (x+1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				tile1.render();
			}
			else {
				tile2.setPos(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				tile2.render();
			}

			//selected square
			if (y == mousePos.y() && x == mousePos.x()) {
				selected.setPos(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				selected.render();
			}
		}
	}
}

