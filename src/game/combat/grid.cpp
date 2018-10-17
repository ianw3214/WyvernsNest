#include "grid.hpp"

#include <cmath> 

Grid::Grid() :
	tilemap(DEFAULT_TILEMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tile1("res/test.png"), 
	tile2("res/test2.png"), 
	selected("res/test3.png") 
{
	// Calculate the tile size based on the screen size
	tile_width = Core::windowWidth() / map_width;
	tile_height = Core::windowHeight() / map_height;
	// Initialize the tile sprites to the tile width/height
	tile1.setSize(tile_width, tile_height);
	tile2.setSize(tile_width, tile_height);
	selected.setSize(tile_width, tile_height);
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
	int x = static_cast<int>(floor(mouseX / tile_width));
	int y = static_cast<int>(floor(mouseY / tile_height));

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

				tile1.setPos(tile_width * x, Core::windowHeight() - tile_height * (y + 1));
				tile1.render();
			}
			else {
				tile2.setPos(tile_width * x, Core::windowHeight() - tile_height * (y + 1));
				tile2.render();
			}

			//selected square
			if (y == mousePos.y() && x == mousePos.x()) {
				selected.setPos(tile_width * x , Core::windowHeight() - tile_height * (y + 1));
				selected.render();
			}
		}
	}
}