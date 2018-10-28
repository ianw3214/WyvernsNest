#include "combat.hpp"

Combat::Combat() :
	current(nullptr)
{
	Player * player1 = new Player();
	Player * player2 = new Player(1, 2);

	player1->setTileSize(grid.tile_width, grid.tile_height);
	player2->setTileSize(grid.tile_width, grid.tile_height);

	addEntity(player1);
	addEntity(player2);

	units.push_back(player1);
	units.push_back(player2);
}

Combat::~Combat() {

}

#include <iostream>
void Combat::handleEvent(const SDL_Event& e) {

	// Only handle events if there is no selected unit or if the selected unit is idle
	if (current == nullptr || current->getState() == UnitState::IDLE) {
		for (Entity * entity : entities) entity->handleEvent(e);

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			// Movement handling logic
			if (grid.isMousePosValid()) {
				Unit * selected = getUnitAt(grid.mousePos);
				// If the selected unit is different from the current one, change it
				if (selected && selected != current) {
					if (current) current->selected = false;
					current = selected;
					// TODO: turn off attacks
				}
				// Otherwise if there is a current unit, move it
				else if (current) {
					if (current->getType() == UnitType::PLAYER) {
						Player * player = dynamic_cast<Player*>(current);
						player->click(grid.mousePos);
						player->selected = false;

						current = player;
					}
					// TODO: do something with the target
				}
			}
		}
	}
}

void Combat::update(int delta) {
	// Update shit here
	grid.update();
	for (Entity * e : entities) e->update(delta);
}

void Combat::render() {

	Core::Renderer::clear();

	if (playerTurn) {
		grid.render();
		for (Entity * e : entities) {

			if (dynamic_cast<Player*>(e) != nullptr) {
				//std::string test = typeid(e).name();
				((Player *)e)->attack1.render(grid.mousePos);
				e->render();

			}
			else {
				//not a player
			}
		}

		//enemies[0].render();

	}
	else {
		Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));

	}


}

Unit * Combat::getUnitAt(ScreenCoord at)
{
	for (Unit * unit : units) {
		if (unit->position.x() == grid.mousePos.x() && unit->position.y() == grid.mousePos.y()) {
			unit->selected = !unit->selected;
			return unit;
		}
	}
	return nullptr;
}
