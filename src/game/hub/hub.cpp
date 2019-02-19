#include "hub.hpp"

#include "util/util.hpp"
#include "../combat.hpp"

#include <utility>
#include <fstream>

Hub::Hub(const std::string& filePath) :
	backdrop(filePath),
	pauseMenu(),
	cursor(),
	player("res/assets/shadow.png"),
	warningSprite("res/assets/UI/BackButton.png")
{
	initSprites();
	initStaticObjects();

	player = HubPlayer("res/assets/shadow.png", Core::windowWidth() / 2, 
						Core::windowHeight() / 2, statObjlist);
	warningSprite.setPos(Core::windowWidth() - 200, Core::windowHeight() / 2 - 150);
}


Hub::~Hub()
{
}

void Hub::initSprites() {
	// resize sprites
	backdrop.setSize(Core::windowWidth(), Core::windowHeight());
}

void Hub::initStaticObjects() {
	// TODO: read objects from json file

	StaticHubObject* test = new StaticHubObject("res/assets/players/MaleBase.png", 
												Core::windowWidth() / 2 - 100, 
												Core::windowHeight() / 2 - 100,
												64, 80);
	statObjlist.push_back(test);
}

void Hub::handleEvent(const SDL_Event& e) {
	if (!pauseMenu.getIsShown()) {
		player.handleEvent(e);
	}

	// Handle pause menu
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
		pauseMenu.setIsShown(true);
	}
	if (pauseMenu.getIsShown()) {
		// Handle cursor events
		cursor.handleEvent(e);

		pauseMenu.handleEvent(e);
	}

	// Handle continue trigger
	if (player.isPlayerTransition()) {
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) {
			// TODO move on to the next proper level
			changeState(new Combat("res/data/levels/level1.json"));
		}
	}
}

void Hub::update(int delta) {
	if (!pauseMenu.getIsShown()) player.update(delta);
	pauseMenu.update(delta);
}

void Hub::render() {
	Core::Renderer::clear();
	backdrop.render();
	player.render();
	pauseMenu.render();
	
	// Render all the static objects
	for (StaticHubObject * statObj : statObjlist) {
		statObj->render();
	}

	// Render an info box to prompt the user to move to the next level
	if (player.isPlayerTransition()) {
		warningSprite.render();
	}

	// Only render the cursor when pausing
	if (pauseMenu.getIsShown()) cursor.render();
}

std::vector<StaticHubObject *> Hub::getStaticObjects() {
	return statObjlist;
}
