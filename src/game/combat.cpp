#include "combat.hpp"

#include "util/attackloader.hpp"
#include "combat/unit.hpp"
#include "combat/player.hpp"
#include "combat/enemy.hpp"

#include "combat/status.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Combat::Combat() :
	current(nullptr)
{
	grid = Grid("res/data/maps/map1.json");

	addPlayer(0, 1);
	addPlayer(1, 2);

	addEnemy(new Enemy(), 2, 2);

	// Keeping track of turn order
	unitIndex = 0;
	// nextUnitTurn();
	selectUnit(units[unitIndex]);

	for (Unit * unit : units) unit->combat = this;
}

Combat::Combat(const std::string & filePath) {

	// Load grid and enemy data from the file path
	std::ifstream file(filePath);
	if (file.is_open()) {
		json data;
		file >> data;

		std::string grid_path = data["map"];
		grid = Grid(std::string("res/data/maps/") + grid_path);

		// TODO: debugging code
		// TODO: Load player from 'PLAYER DATA' file and 'PLAYER POSITION' property in file path
		addPlayer(0, 1);
		addPlayer(1, 2);

		// Load the enemies into the combat state
		json enemies = data["enemies"];
		for (const json& enemy : enemies) {
			std::string type = enemy["type"];
			if (type == "BABY GOOMBA") {
				int x = enemy["x"];
				int y = enemy["y"];
				Enemy * unit = new Enemy();
				addEnemy(unit, x, y);
			}
		}
	}

	// Keeping track of turn order
	unitIndex = 0;
	// nextUnitTurn();
	selectUnit(units[unitIndex]);

	for (Unit * unit : units) unit->combat = this;
}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	for (Entity * entity : entities) entity->handleEvent(e);
	// Handle mouse clicks for player units
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (current->getType() == UnitType::PLAYER) {
			Player * player = dynamic_cast<Player*>(current);
			player->click(grid.mousePos);
		}
	}
	// Keep looking for win/lose conditions while the game hasn't ended
	if (!game_over) {
		// Check for win condition if the player input triggers it
		bool win = true;
		for (const Unit * unit : units) 
			if (unit->getType() == UnitType::ENEMY && unit->health > 0) win = false;
		if (win) {
			// Handle the win condition here
			game_over = true;
			game_win = true;
		}
		// TODO: Also check for lose condition where all player units are dead
	}
	// Otherwise, handle the events for the game over menu
	else {
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_RETURN) {
				// Move on to the next state
				changeState(new Combat());
			}
		}
	}
}


void Combat::update(int delta) {

	grid.update();
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
		
	}
}

void Combat::render() {
	Core::Renderer::clear();
	grid.render();
	{	// --------- UNIT RENDERING CODE ---------
		// Render the bottom of the units first
		for (Unit * unit : units) unit->renderBottom();
		// Render sprites in the order they appear in the grid
		for (int i = 0; i < grid.map_height; ++i) {
			for (Unit * unit : units) {
				if (unit->position.y() == i) {
					unit->render();
				}
			}
		}
		// Render the top of the units now
		for (Unit * unit : units) unit->renderTop();
	}
	// Render the game over screen if the game is over
	if (game_over) {
		// First, render the base rectangle
		int x = (Core::windowWidth() - GAME_OVER_MENU_WIDTH) / 2;
		int y = (Core::windowHeight() - GAME_OVER_MENU_HEIGHT) / 2;
		Core::Renderer::drawRect(ScreenCoord(x, y), GAME_OVER_MENU_WIDTH, GAME_OVER_MENU_HEIGHT, Colour(0.6f, 0.6f, 0.6f));
		// Render text
		Core::Text_Renderer::render("GAME OVER", ScreenCoord(x + 200, y), 4.f);
		y += 220;
		Core::Text_Renderer::render("Press enter to continue", ScreenCoord(x + 180, y));
	}

	/*
    // RENDER SAMPLES
    Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 50), ScreenCoord(2000, Core::windowHeight() - 50), Colour(0.0, 1.0, 0.0));
    Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 66), ScreenCoord(2000, Core::windowHeight() - 66), Colour(0.0, 1.0, 0.0));
    Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 82), ScreenCoord(2000, Core::windowHeight() - 82), Colour(0.0, 1.0, 0.0));
    Core::Text_Renderer::setColour(Colour(0,0,0));
    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Top align.", ScreenCoord(50, 50), 1.f);

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
    Core::Text_Renderer::render("Bottom align.", ScreenCoord(50, 50), 1.f);

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::middle);
    Core::Text_Renderer::render("Middle align.", ScreenCoord(100, 50), 1.f);

    Core::Renderer::drawLine(ScreenCoord(500, 0), ScreenCoord(500, 1000), Colour(0.0, 1.0, 0.0));

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Left align.", ScreenCoord(500, 50), 1.f);

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Centre align.", ScreenCoord(500, 80), 1.f);

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::right, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Right align.", ScreenCoord(500, 110), 1.f);

    Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	*/
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

void Combat::addPlayer(int x, int y) {
	Player * player = new Player(x, y);
	player->setTileSize(grid.tile_width, grid.tile_height);
	addEntity(player);
	units.push_back(player);

	// TODO: Temporary debugging code
	player->addStatus(new BurnStatus(10, 3));
}

void Combat::addEnemy(Enemy * enemy, int x, int y) {
	enemy->position.x() = x;
	enemy->position.y() = y;
	enemy->setTileSize(grid.tile_width, grid.tile_height);

	addEntity(enemy);
	units.push_back(enemy);
	return;
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
	} else {
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

bool Combat::isPosEmpty(Vec2<int> pos) const {
	if (!grid.isPosValid(pos)) return false;
	for (const Unit * unit : units) {
		if (unit->position == pos) {
			return false;
		}
	}
	return true;
}
