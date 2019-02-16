#include "hubPlayer.hpp"


HubPlayer::HubPlayer(const std::string &filePath) : playerSprite(filePath)
{
}


HubPlayer::~HubPlayer()
{
}

void HubPlayer::handleEvent(const SDL_Event& event) {
	// TODO
}

void HubPlayer::update(int delta) {
	// TODO
}

void HubPlayer::render() {
	playerSprite.render();
}