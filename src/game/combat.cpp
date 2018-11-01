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

	unitIndex = 0;
	selectUnit(units[unitIndex]);
}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	// Only handle events if there is no selected unit or if the selected unit is idle
	for (Entity * entity : entities) entity->handleEvent(e);

	/*
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (grid.isMousePosValid()) {
			Unit * selected = getUnitAt(grid.mousePos);
			if (!selected || selected->getType() == UnitType::ENEMY) {
				if (current->getType() == UnitType::PLAYER) {
					Player * player = dynamic_cast<Player*>(current);
					player->click(grid.mousePos, *this);
				}
			}
		}
	}
	*/
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (current->getType() == UnitType::PLAYER) {
			Player * player = dynamic_cast<Player*>(current);
			player->click(grid.mousePos, *this);
		}
	}
}


void Combat::update(int delta) {
	// Update shit here
	grid.update();
	for (Entity * e : entities) e->update(delta);

	// If the unit is done with its state, go to the next unit
	if (current->getState() == UnitState::DONE) {
		nextUnitTurn();
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
