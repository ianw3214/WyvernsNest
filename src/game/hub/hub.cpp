#include "hub.hpp"

#include "util/util.hpp"
#include "../combat.hpp"
#include "../customization.hpp"
#include "../menus/menu.hpp"
#include "../menus/creditsmenu.hpp"

#include <utility>
#include <fstream>
using json = nlohmann::json;

Hub::Hub() : backdrop("res/assets/UI/BackButton.png"),
			 pauseMenu(),
			 cursor(),
			 player("res/assets/hub/mcBase.png"),
			 warningContinueSprite("res/assets/UI/BackButton.png"),
			 warningCustomSprite("res/assets/UI/Continue.png"),
			 warningShopSprite("res/assets/UI/skill_icons/root.png")
{
	// Read the save file to see which hub to load
	std::ifstream old_save(USER_SAVE_LOCATION);
	json inputData;
	old_save >> inputData;
	bool valid = true;
	if (inputData.find("players") == inputData.end()) valid = false;
	if (inputData.find("level") == inputData.end()) valid = false;
	if (inputData.find("hub") == inputData.end()) valid = false;
	if (valid) {
		hub_state = inputData["hub"];
	}
	old_save.close();

	const std::string &hubFilePath = "res/data/hub/hub" + hub_state + ".json";

	initSprites(hubFilePath);
	initStaticObjects(hubFilePath);	

	player = HubPlayer("res/assets/hub/mcBase.png", Core::windowWidth() / 2, 
						Core::windowHeight() / 2, statObjlist);
}


Hub::~Hub()
{
}

void Hub::initSprites(const std::string& filePath) {
	// load the backdrop
	std::ifstream file(filePath);
	if (file.is_open()) {
		json data;
		file >> data;
		last_level = data["lastLevel"];
		backdrop = Sprite(data["backdropPath"]);
	}

	// resize sprites
	backdrop.setSize(Core::windowWidth(), Core::windowHeight());

	// Set position of sprites
	warningContinueSprite.setPos(Core::windowWidth() - 200, Core::windowHeight() / 2 - 150);
	warningCustomSprite.setPos(150, Core::windowHeight() / 2 - 100);
	warningShopSprite.setPos(Core::windowWidth() / 2 - 50, Core::windowHeight() / 2 - 150);
}

void Hub::initStaticObjects(const std::string & filePath) {
	std::ifstream file(filePath);
	if (file.is_open()) {
		json data;
		file >> data;

		for (const json& obj : data["statobj"]) {
			std::string obj_path = obj["path"];
			int obj_xPos = obj["x"];
			int obj_yPos = obj["y"];
			StaticHubObject* test = new StaticHubObject(std::string("res/assets/") + obj_path,
														obj_xPos, obj_yPos);

			statObjlist.push_back(test);

		}
	}
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
	if (player.isPlayerTransitionContinue()) {
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) {
			if (last_level) {
				changeState(new CreditsMenu());
			}
			else 
				switchToCombatState();
		}
	}

	// Handle customization trigger
	if (player.isPlayerTransitionCustom()) {
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) {
			 changeState(new Customization());
		}
	}

	// TODO: Handle shop tigger
	if (player.isPlayerTransitionShop()) {
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) {
			// TODO: move to a shop state
		}
	}
}

void Hub::switchToCombatState() {
	// Assume the save file is always valid
	std::ifstream save_file(DEFAULT_PLAYER_FILE);
	json inputData;
	save_file >> inputData;
	int level_id = inputData["level"];
	level_id++; // add a level to go to the next level

	// Change the state based on the level file
	std::string combatLevelLocation;
	std::ifstream masterFile(DEFAULT_MASTER_FILE);
	json masterData;
	masterFile >> masterData;

	bool level_found = false;
	for (const json& level : masterData["levels"]) {
		if (level["id"] == level_id) {
			std::string name = level["file"];
			combatLevelLocation = std::string("res/data/levels/") + name;
			level_found = true;
		}
	}
	save_file.close();
	masterFile.close();
	if (level_found) {
		// Save the new level to the game
		inputData["level"] = level_id;
		std::ofstream out_save_file(DEFAULT_PLAYER_FILE);
		out_save_file << inputData.dump(4);
		out_save_file.close();
	}
	changeState(new Combat(combatLevelLocation));
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
	
	// Render player hitbox
	Core::Renderer::setRectAlpha(0.5);
	Core::Renderer::drawRect(Vec2<int>(player.getBox().x, player.getBox().y),
		player.getBox().w, player.getBox().h, Vec3<float>(255, 0, 0));

	// Render all the static objects
	for (StaticHubObject * statObj : statObjlist) {
		statObj->render();
		Core::Renderer::drawRect(Vec2<int>(statObj->getBox().x, statObj->getBox().y),
			statObj->getBox().w, statObj->getBox().h, Vec3<float>(255, 0, 0));
	}

	// Render an info box to prompt the user to move to the next level
	if (player.isPlayerTransitionContinue()) {
		warningContinueSprite.render();
	}

	// Render an info box to prompt the user to enter customization
	if (player.isPlayerTransitionCustom()) {
		warningCustomSprite.render();
	}

	// Render an info box to prompt user to enter the shop
	if (player.isPlayerTransitionShop()) {
		warningShopSprite.render();
	}

	// Only render the cursor when pausing
	if (pauseMenu.getIsShown()) cursor.render();
}

std::vector<StaticHubObject *> Hub::getStaticObjects() {
	return statObjlist;
}
