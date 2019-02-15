#include "combat.hpp"

#include "util/util.hpp"

#include "util/attackloader.hpp"
#include "util/particleloader.hpp"
#include "combat/unit.hpp"
#include "combat/player.hpp"
#include "combat/enemy.hpp"
#include "combat/status.hpp"
#include "combat/enemies/mageDudeEnemy.hpp"
#include "combat/enemies/basicWarriorEnemy.hpp"
#include "combat/enemies/babyGoombaEnemy.hpp"


#include "menus/menu.hpp"
#include "menus/creditsmenu.hpp"
#include "customization.hpp"

#include <utility>
#include <fstream>
using json = nlohmann::json;

Combat::Combat(const std::string & filePath, bool last_level) :
	current(nullptr),
	gameOverBase("res/assets/UI/game_over/base.png"),
	pauseBase("res/assets/UI/pauseBase.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png"),
	last_level(last_level)
{
	initSprites();

	// Load grid and enemy data from the file path
	std::ifstream file(filePath);
	if (file.is_open()) {
		json data;
		file >> data;

		std::string grid_path = data["map"];
		grid = Grid(std::string("res/data/maps/") + grid_path);

		// Load the player data from file
		std::ifstream save(USER_SAVE_LOCATION_COMBAT);
		json save_data;
		save >> save_data;
		std::vector<json> player_data = save_data["players"];

		// Load the players into the combat state
		json player_spawns = data["player_spawns"];
		unsigned int index = 0;
		for (const json& spawn : player_spawns) {
			if (index < player_data.size()) {
				addPlayer(spawn["x"], spawn["y"], player_data[index]);
				index++;
			}
		}

		// Load the enemies into the combat state
		json enemies = data["enemies"];
		for (const json& enemy : enemies) {
			std::string type = enemy["type"];
			if (type == "BABY GOOMBA") {
				int x = enemy["x"];
				int y = enemy["y"];
				Enemy * unit = new BabyGoombaEnemy();
				addEnemy(unit, x, y);
			}
			else if (type == "MAGE DUDE") {
				int x = enemy["x"];
				int y = enemy["y"];
				Enemy *unit = new MageDudeEnemy();
				addEnemy(unit, x, y);
			}
			else if (type == "BASIC WARRIOR") {
				int x = enemy["x"];
				int y = enemy["y"];
				Enemy *unit = new WarriorEnemy();
				addEnemy(unit, x, y);
			}
		}

		// Load the rewards
		json reward= data["reward"];
		experienceReward = reward["experience"];
		if (reward.find("units") != reward.end()) {
			for (json unit : reward["units"]) {
				std::string name = unit["name"];
				std::string attack1 = unit["attack1"];
				std::string attack2 = unit["attack2"];
				std::string attack3 = unit["attack3"];
				std::string attack4 = unit["attack4"];
				UnitData data;
				data.name = name;
				data.strength = unit["STR"];
				data.dexterity = unit["DEX"];
				data.intelligence = unit["INT"];
				data.constitution = unit["CON"];
				data.attack1 = attack1;
				data.attack2 = attack2;
				data.attack3 = attack3;
				data.attack4 = attack4;
				data.level = unit["level"];
				data.experience = unit["experience"];
				unitRewards.push_back(data);
			}
		}
	}
	for (Unit * unit : units) unit->combat = this;

	// Initialize the particle system
	ps = ParticleSystem();

	// Start the game by selecting the first unit to go
	startGame();

}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	// If the combat state isn't paused, look for gameplay events
	if (!pause) {
		// Keep looking for win/lose conditions while the game hasn't ended
		if (!game_over) {
			#ifdef _DEBUG
			// !! DEBUG ONLY !! - Add a way to circumvent the level
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_9) {
				for (Unit * u : units) {
					if (u->getType() == UnitType::ENEMY) {
						u->health = 0;
					}
				}
			}
			#endif
			// Check for win condition if the player input triggers it
			updateWinStatus();
		}
		// Otherwise, handle the events for the game over menu
		else {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
					if (render_game_over) {
						// Move on to the next state
						if (last_level) {
							changeState(new CreditsMenu());
						} else {
							changeState(new Customization());
						}
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				ScreenCoord mousePos;
				SDL_GetMouseState(&mousePos.x(), &mousePos.y());
				if (continueButton.colliding(mousePos)) {
					if (game_win) {
						// Move on to the next state
						if (last_level) {
							changeState(new CreditsMenu());
						} else {
							changeState(new Customization());
						}
					} else {
						changeState(new Menu());
					}
				}
			}
		}
		// Pause the game if escape key is pressed
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			pause = true;
			// If the player wasn't in the middle of attacking, show the pause menu
			if (current->getType() == UnitType::PLAYER) {
				Player * player = dynamic_cast<Player*>(current);
				if (player->player_state == PlayerState::ATTACKING) {
					pause = false;
				}
			}
		}
		// Update entities after updating combat state because pause depends on player state
		for (Entity * entity : entities) entity->handleEvent(e);
	}
	// Pause menu event handling
	else {
		if (e.type == SDL_MOUSEBUTTONUP) {
			ScreenCoord mouse;
			SDL_GetMouseState(&mouse.x(), &mouse.y());
			if (resumeButton.colliding(mouse)) {
				pause = false;
			}
			if (menuButton.colliding(mouse)) {
				changeState(new Menu());
			}
			if (quitButton.colliding(mouse)) {
				exit(0);
			}
		}
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			exit(0);
		}
	}
	// Update the mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
	}
}

void Combat::update(int delta) {
	if (!pause) {
		grid.update();
		ps.update();
		for (Entity * e : entities) e->update(delta);

		if (current->getType() == UnitType::PLAYER && current->getState() == UnitState::IDLE) {
			Player * player = dynamic_cast<Player*>(current);
			player->setPathLine(grid.getMouseToGrid());
		}

		// If the game isn't over, keep going with the turn order
		if (!game_over) {
			// If the unit is done with its state, go to the next unit
			if (current->getState() == UnitState::DONE || current->getState() == UnitState::DEAD) {
				nextUnitTurn();
			}
		}
		// If the game is over, update according to the menu
		else {
			if (compareCounter(GAME_OVER_TIMER)) {
				render_game_over = true;
			} else {
				incrementCounter();
			}
		}
	}
}

void Combat::render() {
	Core::Renderer::clear();
	grid.render();
	{	// --------- UNIT RENDERING CODE ---------
		// Render the bottom of the units first
		for (Unit * unit : units) unit->renderBottom(this);
		// Render sprites in the order they appear in the grid
		for (int i = 0; i < grid.map_height; ++i) {
			for (Unit * unit : units) {
				if (unit->position.y() == i) {
					unit->render();
				}
			}
		}
		// Render the top of the units now
		for (Unit * unit : units) unit->renderTop(this);
	}
	// Render the game over screen if the game is over
	if (render_game_over) {
		// First, render the base rectangle
		gameOverBase.render();
		// Render text based on whether the player won or not
		if (game_win) {		// Player won
			Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::bottom);
			Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
			Core::Text_Renderer::render("VICTORY", ScreenCoord(SubDiv::hCenter(), SubDiv::vPos(10, 3)), 2.5f);
			unsigned int i = 0;
			// Render the player stats
			for (; i < gameOverData.size(); ++i) {
				Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
				Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
				Core::Text_Renderer::render(gameOverData[i].name, ScreenCoord(SubDiv::hPos(12, 3), SubDiv::vPos(5, 2) + SubDiv::vSize(12, i)), 1.5f);
				Core::Text_Renderer::setColour(Colour(.5f, .3f, .2f));
				std::string text = std::string("+") + std::to_string(gameOverData[i].gained_exp) + (" EXP");
				Core::Text_Renderer::render(text, ScreenCoord(SubDiv::hPos(2, 1), SubDiv::vPos(5, 2) + SubDiv::vSize(12, i)), 1.f);
				if (gameOverData[i].level_up) {
					Core::Text_Renderer::setColour(Colour(.9f, 1.f, .9f));
					Core::Text_Renderer::render("LEVEL UP!", ScreenCoord(SubDiv::hPos(24, 15), SubDiv::vPos(5, 2) + SubDiv::vSize(12, i)), 1.2f);
				}
			}
			// Render gained units
			for (; i < gameOverData.size() + unitRewards.size(); ++i) {
				int j = i - gameOverData.size();
				Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
				Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
				Core::Text_Renderer::render(unitRewards[j].name, ScreenCoord(SubDiv::hPos(12, 3), SubDiv::vPos(5, 2) + SubDiv::vSize(12, i)), 1.5f);
				Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
				Core::Text_Renderer::render("NEW UNIT!", ScreenCoord(SubDiv::hPos(24, 15), SubDiv::vPos(5, 2) + SubDiv::vSize(12, i)), 1.2f);
			}
			// Render the continue button
			continueButton.render();
			Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
			Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::middle);
			Core::Text_Renderer::render("CONTINUE", continueButton.position + ScreenCoord(80, 35), 1.3f);
		}
		else {				// Player lost
			Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::bottom);
			Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
			Core::Text_Renderer::render("DEFEAT", ScreenCoord(SubDiv::hCenter(), SubDiv::vPos(10, 3)), 2.5f);
			continueButton.render();
			Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
			Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::middle);
			Core::Text_Renderer::render("BACK", continueButton.position + ScreenCoord(80, 35), 1.3f);
		}
	}

	ps.render();

	// If the game is paused, render the pause menu
	if (pause) {
		pauseBase.render();
		Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::bottom);
		Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
		Core::Text_Renderer::render("PAUSE", ScreenCoord(SubDiv::hCenter(), SubDiv::vPos(10, 3)), 2.5f);
		Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
		Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::middle);
		resumeButton.render();
		Core::Text_Renderer::render("RESUME", resumeButton.position + Vec2<int>(120, 32), 1.8f);
		menuButton.render();
		Core::Text_Renderer::render("MAIN MENU", menuButton.position + Vec2<int>(120, 32), 1.8f);
		quitButton.render();
		Core::Text_Renderer::render("QUIT", quitButton.position + Vec2<int>(120, 32), 1.8f);
	}

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	} else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

// Returns the unit occupying the specified grid coordinate
Unit * Combat::getUnitAt(ScreenCoord at)
{
	for (Unit * unit : units) {
		if (unit->position.x() == at.x() && unit->position.y() == at.y()) {
			return unit;
		}
	}
	return nullptr;
}

std::vector<Player*> Combat::getPlayers() const {
	std::vector<Player*> result;
	for (Unit * unit : units) {
		if (unit->getType() == UnitType::PLAYER) {
			result.push_back(dynamic_cast<Player*>(unit));
		}
	}
	return result;
}

void Combat::addPlayer(int x, int y, const json& data) {
	Player * player = new Player(x, y, data);
	player->setTileSize(grid.tile_width, grid.tile_height);
	addEntity(player);
	units.push_back(player);
}

void Combat::addEnemy(Enemy * enemy, int x, int y) {
	enemy->position.x() = x;
	enemy->position.y() = y;
	enemy->setTileSize(grid.tile_width, grid.tile_height);

	addEntity(enemy);
	units.push_back(enemy);
	return;
}

void Combat::initSprites() {
	// Calculate game over menu sprite position
	int width = gameOverBase.getTexture().getWidth();
	int height = gameOverBase.getTexture().getHeight();
	gameOverBase.setPos((Core::windowWidth() - width) / 2, (Core::windowHeight() - height) / 2);
	// Calculate pause menu sprite position
	width = pauseBase.getTexture().getWidth();
	height = pauseBase.getTexture().getHeight();
	pauseBase.setPos((Core::windowWidth() - width) / 2, (Core::windowHeight() - height) / 2);

	continueButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vPos(40, 27)));
	resumeButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter() - 100), 240);
	menuButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter()), 240);
	quitButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter() + 100), 240);
}

// Choose the next unit in combat to take a turn
void Combat::nextUnitTurn() {
	// Increment the unit index to get the next logical unit in combat
	unitIndex++;
	if (unitIndex == units.size()) {
		unitIndex = 0;
	}
	// Skip the units turn if its dead
	if (units[unitIndex]->getState() == UnitState::DEAD) {
		nextUnitTurn();
		return;
	}
	else {
		selectUnit(units[unitIndex]);
	}
	// If the current unit is an enemy, take its turn
	if (current->getType() == UnitType::ENEMY) {
		dynamic_cast<Enemy*>(current)->takeTurn();
	}
}

// Select a unit to be the current unit
void Combat::selectUnit(Unit * unit)
{
	if (current) {
		current->setState(UnitState::IDLE);
		current->deselect();
	}
	current = unit;
	unit->select();

}

void Combat::startGame() {
	// Keeping track of turn order
	unitIndex = 0;

	std::sort(units.begin(), units.end(), greater_than_DEX());

	selectUnit(units[unitIndex]);
	// If the first unit is an enemy, take its turn
	if (current->getType() == UnitType::ENEMY) {
		dynamic_cast<Enemy*>(current)->takeTurn();
	}

}

void Combat::updateWinStatus() {
	bool win = true;
	bool lose = true;
	for (const Unit * unit : units) {
		if (unit->getType() == UnitType::ENEMY && unit->health > 0) win = false;
		if (unit->getType() == UnitType::PLAYER && unit->health > 0) lose = false;
	}
	if (win) {
		game_over = true;
		game_win = true;

		// Save updated information to the save file
		std::ifstream old_save(USER_SAVE_LOCATION);
		json inputData;
		old_save >> inputData;
		bool valid = true;
		if (inputData.find("players") == inputData.end()) valid = false;
		if (inputData.find("level") == inputData.end()) valid = false;
		if (!valid) {
			// TODO: something here, save is no good
		} else {
			int playerCount = 0;
			for (const json& unit : inputData["players"]) {
				playerCount += 1;
			}
			// Give each player their corresponding amount of experience
			float expPerPlayer = experienceReward / (float)playerCount;
			std::vector<json> updatedPlayers;
			for (json unit : inputData["players"]) {
				std::string name = unit["name"];
				bool level_up = false;
				int currentExp = unit["experience"];
				int newExp = static_cast<int>(currentExp + expPerPlayer);
				if (newExp >= DEFAULT_MAX_EXP) {
					unit["level"] += 1;
					unit["experience"] = newExp - DEFAULT_MAX_EXP;
					unit["STR"] += 2;
					unit["DEX"] += 2;
					unit["INT"] += 2;
					unit["CON"] += 2;
					level_up = true;
				} else {
					unit["experience"] = newExp;
				}
				updatedPlayers.push_back(unit);
				gameOverData.push_back(GameOverData{ name, static_cast<int>(expPerPlayer), level_up });
			}
			// Add the new units to the updated players
			for (UnitData unit : unitRewards) {
				json res;
				res["name"] = unit.name;
				res["STR"] = unit.strength;
				res["DEX"] = unit.dexterity;
				res["INT"] = unit.intelligence;
				res["CON"] = unit.constitution;
				res["attack1"] = unit.attack1;
				res["attack2"] = unit.attack2;
				res["attack3"] = unit.attack3;
				res["attack4"] = unit.attack4;
				res["level"] = unit.level;
				res["experience"] = unit.experience;
				// Initialize the skill tree to default (root)
				std::vector<int> selected;
				selected.push_back(1);
				res["selected"] = selected;
				updatedPlayers.push_back(res);
			}
			old_save.close();
			json outputData;
			outputData["level"] = inputData["level"];
			outputData["players"] = updatedPlayers;
			std::ofstream new_save(USER_SAVE_LOCATION);
			new_save << outputData.dump(4);
		}
		// Start the counter to show the death menu
		startCounter();
	}
	// TODO: Also check for lose condition where all player units are dead
	if (!win && lose) {
		// Handle the win condition here
		game_over = true;
		game_win = false;
		// Start the counter to show the death menu
		startCounter();
	}
}

bool Combat::isPosEmpty(Vec2<int> pos) const {
	if (!grid.isPosValid(pos)) return false;
	for (const Unit * unit : units) {
		if (unit->position == pos) {
			return false;
		}
	}
	return true;
}

void Combat::addEmitter(Emitter * emitter) {
	ps.addEmitter(emitter);
}
