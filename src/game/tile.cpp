#include "tile.hpp"



Tile::Tile(const std::string filePath, int tileType) :
	mtileSprite(filePath)
{
	mtileType = tileType;
	debufApply();
}

Tile::~Tile()
{
}

void Tile::debufApply() {
	if (mtileType == 0) { // PLAIN
		mdebufDEX = 0;
	}
	else if (mtileType == 1) { // POND
 		mdebufDEX = 1;
	}
}

void Tile::handleEvent(const SDL_Event& e) {
	// TODO
}

void Tile::update(int delta) {
	// TODO
}

void Tile::render() {
	// TODO
}

int Tile::getDebufDEX() {
	return mdebufDEX;
}

int Tile::getTileType() {
	return mtileType;
}