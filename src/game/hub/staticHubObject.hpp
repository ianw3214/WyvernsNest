#pragma once

#include "../../engine/core.hpp"

class StaticHubObject : public Entity
{
public:
	StaticHubObject(const std::string filePath, int x, int y, int width, int height);
	~StaticHubObject();

	// Override entity functions
	void handleEvent(const SDL_Event& event) override;
	void update(int delta) override;
	void render() override;

	// Get collision box
	SDL_Rect getBox();

private:
	// The collision box
	SDL_Rect mBox;

	// The object Sprite
	Sprite objSprite;
};

