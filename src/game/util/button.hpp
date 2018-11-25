#pragma once

#include <string>

#include "../engine/core.hpp"

class ButtonData {
public:
	Vec2<int> position;
	int width;
	int height;

	ButtonData(Vec2<int> position = Vec2<int>(0, 0), int width = 160, int height = 80);
	~ButtonData();

	void setSprites(const std::string& defaultSpr, const std::string& hoverSpr, const std::string& pressedSpr);

	void render();
	bool colliding(ScreenCoord point);

	Sprite default_sprite;
	Sprite hover_sprite;
};