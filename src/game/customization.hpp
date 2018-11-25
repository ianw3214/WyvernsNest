#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"

#define DEFAULT_PLAYER_FILE		"res/data/players.json"

class ButtonData {
public:
	Vec2<int> position;
	int width;
	int height;

	ButtonData(Vec2<int> position = Vec2<int>(0, 0), int width = 160, int height = 80);
	~ButtonData();

	void render();
	bool colliding(ScreenCoord point);

	Sprite default_sprite;
};

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

	// Buttons used to bring to next state
	ButtonData button1;
	ButtonData button2;
	ButtonData button3;
	ButtonData button4;
	
	void initSprites();
	// Sprites used to render the customization state
	Sprite base;
	Sprite empty;

};