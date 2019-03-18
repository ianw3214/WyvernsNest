#pragma once

#include "../engine/core.hpp"

class Tile : public Entity
{
public:
	Tile(const std::string filePath, int tileType);
	~Tile();

	// override engine functions
	void handleEvent(const SDL_Event& event) override;
	void update(int delta) override;
	void render() override;

	// Set tile position
	void setTilePosition(int x, int y);
	void setTileSize(int width, int height);

	// Dexterity debuf stats getter
	int getDebufDEX();
	int getTileType() const;

private:
	// Helper function to identify tile type
	void debufApply();

	int mtileType;
	Sprite mtileSprite;

	// Tile position
	int xPos;
	int yPos;

	// Tile size
	int h, w;

	int mdebufDEX;
};

