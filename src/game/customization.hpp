#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"

#define DEFAULT_PLAYER_FILE		"res/data/players.json"

class Customization : public State {

public:

	Customization(const std::string& file = DEFAULT_PLAYER_FILE);
	~Customization();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

	std::vector<UnitData> units;

	void displayUnitData(const UnitData& data);
	void renderUnit(int x, int y, UnitData unit);
	void renderEmpty(int x, int y);
	void generateDefaultUnitData();


	void initSprites();
	// Sprites used to render the customization state
	Sprite base;
	Sprite empty;

};