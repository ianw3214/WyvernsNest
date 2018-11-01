#include "combat.hpp"

Combat::Combat() :
	current(nullptr)
{
	Player * player1 = new Player(0, 1);
	Player * player2 = new Player(1, 2);

	Enemy * enemy1 = new Enemy();
	enemy1->position.x() = 2;
	enemy1->position.y() = 2;

	player1->setTileSize(grid.tile_width, grid.tile_height);
	player2->setTileSize(grid.tile_width, grid.tile_height);
	enemy1->setTileSize(grid.tile_width, grid.tile_height);

	addEntity(player1);
	addEntity(player2);
	addEntity(enemy1);

	units.push_back(player1);
	units.push_back(player2);
	units.push_back(enemy1);

	// Keeping track of turn order
	unitIndex = 0;
	selectUnit(units[unitIndex]);
}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	for (Entity * entity : entities) entity->handleEvent(e);
	// Handle mouse clicks for player units
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (current->getType() == UnitType::PLAYER) {
			Player * player = dynamic_cast<Player*>(current);
			player->click(grid.mousePos, *this);
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

			}
		}
	}
}


void Combat::update(int delta) {

	grid.update();
	for (Entity * e : entities) e->update(delta);

	// If the game isn't over, keep going with the turn order
	if (!game_over) {
		// If the unit is done with its state, go to the next unit
		if (current->getState() == UnitState::DONE) {
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
	// Render sprites in the order they appear in the grid
	for (int i = 0; i < grid.map_height; ++i) {
		for (Unit * unit : units) {
			if (unit->position.y() == i) {
				unit->render();
			}
		}
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

// Returns the next logical unit in combat
void Combat::nextUnitTurn()
{
	unitIndex++;
	if (unitIndex == units.size()) {
		unitIndex = 0;
	}
	selectUnit(units[unitIndex]);
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
		current->selected = false;
	}

	current = unit;
	unit->selected = true;
}

bool Combat::isPosEmpty(Vec2<int> pos) const {
	if (!grid.isPosEmpty(pos)) return false;
	for (const Unit * unit : units) {
		if (unit->position == pos) {
			return false;
		}
	}
	return true;
}
