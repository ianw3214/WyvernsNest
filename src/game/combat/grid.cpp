#include "grid.hpp"

#include <cmath> 
#include <set>
#include <fstream>

// File JSON handling
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Grid::Grid() :
	tilemap(DEFAULT_TILEMAP),
	collisionmap(DEFAULT_COLLISIONMAP),
	map_width(DEFAULT_MAP_WIDTH),
	map_height(DEFAULT_MAP_HEIGHT),
	tilesheet("res/assets/tiles/tilesheet1.png")
{
	init(SOURCE_TILE_WIDTH);
}

// Constructor to load the grid from file data
// TODO: Add collision map loading in file data
Grid::Grid(std::string file) : tilesheet("INVALID") {
	std::ifstream inp(file);
	// Use the provided overloaded operators to lead the json data
	json data;
	inp >> data;

	// Get the map width and height
	map_width  = data["width"];
	map_height = data["height"];
	// Load the tilesheet data
	tilesheet = Sprite(data["tilesheet"]);
	source_width = data["tilesheet_width"];
	source_height = data["tilesheet_height"];

	std::set<int> colIndices = { 2, 4, 6 };
	for (int tile : data["tilemap"]) {
		tilemap.push_back(tile);
		collisionmap.push_back(colIndices.find(tile) == colIndices.end() ? 0 : 1);
	}

	// Initialize other grid attributes based on current map attributes
	init(data["tilewidth"]);
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

// TODO: ONE OF THESE FUNCTIONS SHOULD BE DEPRECATED
bool Grid::isPosEmpty(Vec2<int> pos) const {
	if (TILE_INDEX(pos.x(), pos.y()) < 0) return false;
	if (TILE_INDEX(pos.x(), pos.y()) >= map_width * map_height) return false;
	return (collisionmap[TILE_INDEX(pos.x(), pos.y())] == 0);
}

bool Grid::isPosValid(Vec2<int> pos) const {
	if (pos.x() < 0 || pos.x() >= map_width) return false;
	if (pos.y() < 0 || pos.y() >= map_height) return false;
	return isPosEmpty(pos) && tilemap[TILE_INDEX(pos.x(), pos.y())] != 21;
}

// TODO: Add option to vary source tile width/height
// TODO: Add option to load collision map through file
void Grid::init(int source_tile_width) {
	// Calculate the tile size based on the screen size
	tile_width = Core::windowWidth() / map_width;
	tile_height = (Core::windowHeight() / map_height) + 1;
	// Initialize the tile sprites to the tile width/height
	tilesheet.setSourceSize(source_tile_width, source_tile_width);
	tilesheet.setSize(tile_width, tile_height);
	source_tile_size = source_tile_width;

	// Debugging code
	renderOutline = true;
}

int Grid::indexToX(int index) const {
	return index % source_width;
}

int Grid::indexToY(int index) const {
	return source_height - (index / source_width) - 1;
}

void Grid::render()
{
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			int index = tilemap[TILE_INDEX(x, y)];
			tilesheet.setSourcePos(indexToX(index) * source_tile_size, indexToY(index) * source_tile_size);
			tilesheet.setPos(tile_width * x, tile_height * y);
			tilesheet.render();
		}
	}

	int thickness = 5;

	// Render the debugging outline if set
	if (renderOutline) {
		Core::Renderer::setAlpha(.10f + .07f * sin(Core::getSeconds()));
		// Render horizontal lines
		for (int i = 1; i < map_height; ++i) {
		Core::Renderer::drawRect(
				ScreenCoord(0, tile_height * i),
				Core::windowWidth(),
				thickness,
				Colour(1.f, 1.f, 1.f));
		}
		// Render vertical lines
		for (int i = 0; i < map_width; ++i) {
			Core::Renderer::drawRect(
				ScreenCoord(tile_width * i, 0),
				thickness,
				Core::windowHeight(),
				Colour(1.f, 1.f, 1.f));
		}
		Core::Renderer::setAlpha(1);
	}
}