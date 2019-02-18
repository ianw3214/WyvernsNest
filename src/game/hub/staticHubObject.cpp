#include "staticHubObject.hpp"


StaticHubObject::StaticHubObject(const std::string filePath, int x, int y, int width, 
								int height) :
	objSprite(filePath)
{
	// Set the offsets
	mBox.x = x; mBox.y = y;

	// Set the collision box
	mBox.w = width; mBox.h = height;

	objSprite.setPos(mBox.x, mBox.y);
}


StaticHubObject::~StaticHubObject()
{
}

void StaticHubObject::handleEvent(const SDL_Event& e) {
	// This object is static. No user input should do something!
}

void StaticHubObject::update(int delta) {
	objSprite.setPos(mBox.x, mBox.y);
}

void StaticHubObject::render() {
	objSprite.render();
}

SDL_Rect StaticHubObject::getBox() {
	return mBox;
}