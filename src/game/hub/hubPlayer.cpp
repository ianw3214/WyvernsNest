#include "hubPlayer.hpp"


HubPlayer::HubPlayer(const std::string &filePath, int x, int y) :
	playerSprite(filePath)
{
	xPos = x;
	yPos = y;
	xVel = 0;
	yVel = 0;
	playerSprite.setPos(xPos, yPos);
}


HubPlayer::~HubPlayer()
{
}

void HubPlayer::handleEvent(const SDL_Event& e) {
	// If key was pressed down
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		// Adjust the velocity
		switch(e.key.keysym.sym) {
		case SDLK_w:
			yVel -= PLAYER_VEL;
			break;
		case SDLK_s:
			yVel += PLAYER_VEL;
			break;
		case SDLK_a:
			xVel -= PLAYER_VEL;
			break;
		case SDLK_d:
			xVel += PLAYER_VEL;
			break;
		}
	}

	// If key was released
	if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		// Adjust the velocity
		switch(e.key.keysym.sym) {
		case SDLK_w:
			yVel += PLAYER_VEL;
			break;
		case SDLK_s:
			yVel -= PLAYER_VEL;
			break;
		case SDLK_a:
			xVel += PLAYER_VEL;
			break;
		case SDLK_d:
			xVel -= PLAYER_VEL;
			break;
		}
	}
}

void HubPlayer::move(int xlim, int ylim) {
	// Player horizontal movement
	xPos += xVel;

	// Enforce boundary conditions
	if ((xPos < 0) || (xPos + PLAYER_WIDTH > xlim)) {
		xPos -= xVel;
	}

	// Player vertical movement
	yPos += yVel;

	// Enforce boundary conditions
	if ((yPos < 0) || (yPos + PLAYER_HEIGHT > ylim)) {
		yPos -= yVel;
	}
}

void HubPlayer::update(int delta) {
	move(Core::windowWidth(), Core::windowHeight());
	playerSprite.setPos(xPos, yPos);
}

void HubPlayer::render() {
	playerSprite.render();
}
