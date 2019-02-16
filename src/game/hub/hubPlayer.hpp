#pragma once

#include "../engine/core.hpp"

class HubPlayer : public Entity
{
public:
	HubPlayer(const std::string &filePath);
	~HubPlayer();

	// override entity functions
	void handleEvent(const SDL_Event& event) override;
	void update(int delta);
	void render();

private:
	Sprite playerSprite;
};

