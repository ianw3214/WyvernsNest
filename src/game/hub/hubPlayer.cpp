#include "hubPlayer.hpp"

#include <vector>

HubPlayer::HubPlayer(const std::string &filePath) :
	playerSprite(filePath)
{
}

HubPlayer::HubPlayer(const std::string &filePath, int x, int y,
					std::vector<StaticHubObject *> obj) :
	statObjList(obj),
	playerSprite(filePath)
{
	xPos = x;
	yPos = y;
	xVel = 0;
	yVel = 0;
	playerSprite.setPos(xPos, yPos);

	// Set collision box dimensions
	mCollision.w = PLAYER_WIDTH;
	mCollision.h = PLAYER_HEIGHT;
}

// Helper function to check player collision
bool checkCollision(SDL_Rect a, SDL_Rect b) {
	// Sides of the rectangles
	int leftA, rightA, bottomA, topA;
	int leftB, rightB, bottomB, topB;

	// Compute sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	// Compute sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	// If any of the sides of A are outside of B
	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	return true;
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
	mCollision.x = xPos;

	// Enforce boundary conditions
	if ((xPos < 0) || (xPos + PLAYER_WIDTH > xlim)) {
		xPos -= xVel;
		mCollision.x = xPos;
	}

	// Check if it hit a static object
	for (StaticHubObject * statObj : statObjList) {
		if (checkCollision(statObj->getBox(), mCollision)) {
			// Move back
			xPos -= xVel;
			mCollision.x = xPos;
		}
	}

	// Player vertical movement
	yPos += yVel;
	mCollision.y = yPos;

	// Enforce boundary conditions
	if ((yPos < 0) || (yPos + PLAYER_HEIGHT > ylim)) {
		yPos -= yVel;
		mCollision.y = yPos;
	}

	// Check if it hit a static object
	for (StaticHubObject * statObj : statObjList) {
		if (checkCollision(statObj->getBox(), mCollision)) {
			// Move back
			yPos -= yVel;
			mCollision.y = yPos;
		}
	}
}

void HubPlayer::update(int delta) {
	move(Core::windowWidth(), Core::windowHeight());
	playerSprite.setPos(xPos, yPos);
}

void HubPlayer::render() {
	playerSprite.render();
}

int HubPlayer::getPosX() {
	return xPos;
}

int HubPlayer::getPosY() {
	return yPos;
}

bool HubPlayer::isPlayerTransition() {
	int leftBorder = Core::windowWidth() - 100;
	int rightBorder = Core::windowWidth();
	int topBorder = Core::windowHeight() / 2 - 100;
	int bottomBorder = Core::windowHeight() / 2 + 100;

	if (xPos >= leftBorder && xPos <= rightBorder && yPos <= bottomBorder && yPos >= topBorder) {
		return true;
	}
	return false;
}