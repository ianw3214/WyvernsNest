#include "grid.hpp"

#include <cmath> 

#include <iostream>
Grid::Grid() :
	tilemap(DEFAULT_TILEMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tilesheet("res/assets/tiles/tilesheet1.png")
{
	// Calculate the tile size based on the screen size
	tile_width = Core::windowWidth() / map_width;
	tile_height = Core::windowHeight() / map_height;
	// Initialize the tile sprites to the tile width/height
	tilesheet.setSourceSize(SOURCE_TILE_WIDTH, SOURCE_TILE_HEIGHT);
	tilesheet.setSize(tile_width, tile_height);

	// Fill the grid with no collisions and add the buffer space on top
	for (unsigned int i = 0; i < tilemap.size(); ++i) {
		if (i < static_cast<unsigned int>(map_width)) collisionmap.push_back(true);
		else collisionmap.push_back(false);
	}

	// Debugging code
	renderOutline = true;
}

Grid::~Grid() {

}

void Grid::update()
{
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

bool Grid::isPosEmpty(Vec2<int> pos) const {
	if (TILE_INDEX(pos.x(), pos.y()) < 0) return false;
	if (TILE_INDEX(pos.x(), pos.y()) >= map_width * map_height) return false;
	return !collisionmap[TILE_INDEX(pos.x(), pos.y())];
}

bool Grid::isPosValid(Vec2<int> pos) const {
	return isPosEmpty(pos) && tilemap[TILE_INDEX(pos.x(), pos.y())] != 21;
}

void Grid::render()
{
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			int index = tilemap[TILE_INDEX(x, y)];
			tilesheet.setSourcePos(INDEX_TO_X(index) * SOURCE_TILE_WIDTH, INDEX_TO_Y(index) * SOURCE_TILE_HEIGHT);
			tilesheet.setPos(tile_width * x, tile_height * y);
			tilesheet.render();
		}
	}
	// Render the debugging outline if set
	if (renderOutline) {
		// Render horizontal lines
		for (int i = 1; i < map_height; ++i) {
			Core::Renderer::drawLine(
				ScreenCoord(0, tile_height * i), 
				ScreenCoord(Core::windowWidth(), tile_height * i), 
				Colour(1.f, 1.f, 1.f));
		}
		// Render vertical lines
		for (int i = 0; i < map_width; ++i) {
			Core::Renderer::drawLine(
				ScreenCoord(tile_width * i, 0),
				ScreenCoord(tile_width * i, Core::windowHeight()),
				Colour(1.f, 1.f, 1.f));
		}
	}
}