#pragma once

#include "../engine/core.hpp"

#define PLAYER_VEL 10
#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 5


class HubPlayer : public Entity
{
public:
	HubPlayer(const std::string &filePath, int x, int y);
	~HubPlayer();

	// override entity functions
	void handleEvent(const SDL_Event& event) override;
	void update(int delta) override;
	void render() override;

	// Helper function for player position
	void move(int xlim, int ylim);

private:
	Sprite playerSprite;

	// Player position
	int xPos, yPos;

	// Player velocity
	int xVel, yVel;
};
