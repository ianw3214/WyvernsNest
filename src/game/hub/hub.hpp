#pragma once

#include "../engine/core.hpp"
#include <nlohmann/json.hpp>

class Hub : public State
{
public:
	Hub(const std::string& filePath);
	~Hub();

	// State functions unchanged
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	// Init Sprites
	void initSprites();

private:
	// Backdrop
	Sprite backdrop;
};

