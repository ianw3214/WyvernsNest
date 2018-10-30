#include "grid.hpp"

#include <cmath> 

Grid::Grid() :
	tilemap(DEFAULT_TILEMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tilesheet("res/assets/Tiles.png"), 
	selected("res/test3.png") 
{
	// Calculate the tile size based on the screen size
	tile_width = Core::windowWidth() / map_width;
	tile_height = Core::windowHeight() / map_height;
	// Initialize the tile sprites to the tile width/height
	selected.setSize(tile_width, tile_height);
	tilesheet.setSourceSize(SOURCE_TILE_WIDTH, SOURCE_TILE_HEIGHT);
	tilesheet.setSize(tile_width, tile_height);
}

Grid::~Grid() {

}

Grid::Grid(std::vector<Player>* p, std::vector<Enemy>* e) :
	tilemap(DEFAULT_TILEMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tilesheet("res/assets/Tiles.png"),
	selected("res/test3.png")
{
	// Calculate the tile size based on the screen size
	tile_width = Core::windowWidth() / map_width;
	tile_height = Core::windowHeight() / map_height;
	// Initialize the tile sprites to the tile width/height
	selected.setSize(tile_width, tile_height);
	tilesheet.setSourceSize(SOURCE_TILE_WIDTH, SOURCE_TILE_HEIGHT);
	tilesheet.setSize(tile_width, tile_height);
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
				tilesheet.setSourcePos(0, 0);
				tilesheet.setPos(tile_width * x, tile_height * (y));
				tilesheet.render();
			}
			else {
				tilesheet.setSourcePos(1 * SOURCE_TILE_WIDTH, 0);
				tilesheet.setPos(tile_width * x, tile_height * (y));
				tilesheet.render();
			}

			/*	RENDER THE SELECTED TILE
			if (y == mousePos.y() && x == mousePos.x()) {
				selected.setPos(tile_width * x , Core::windowHeight() - tile_height * (y + 1));
				selected.render();
			}
			*/
		}
	}
}