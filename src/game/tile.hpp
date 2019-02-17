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

	// Dexterity debuf stats getter
	int getDebufDEX();
	int getTileType();

private:
	// Helper function to identify tile type
	void debufApply();

	int mtileType;
	Sprite mtileSprite;

	int mdebufDEX;
};

