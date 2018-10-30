#include "combat.hpp"

Combat::Combat() :
	current(nullptr)
{
	Player * player1 = new Player();
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
}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	// Only handle events if there is no selected unit or if the selected unit is idle
	if (current == nullptr || current->getState() == UnitState::IDLE) {
		for (Entity * entity : entities) entity->handleEvent(e);

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			// Movement handling logic
			if (grid.isMousePosValid()) {
				Unit * selected = getUnitAt(grid.mousePos);

				// If the unit is a player that isn't idle, handle its click
				if (current && 
					current->getType() == UnitType::PLAYER && 
					dynamic_cast<Player*>(current)->current_action != PlayerAction::NONE)
				{
					Player * player = dynamic_cast<Player*>(current);
					player->click(grid.mousePos, *this);

					player->selected = false;
					current = nullptr;
				} else {
					// If the selected unit is different from the current one, change it
					if (selected && selected != current) {
						if (current) {
							current->selected = false;
						}
						current = selected;
					}
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
			e->render();
		}
    // RENDER SAMPLES
    /*
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

		//enemies[0].render();
	}
	else {
		Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));

	}


}

Unit * Combat::getUnitAt(ScreenCoord at)
{
	for (Unit * unit : units) {
		if (unit->position.x() == at.x() && unit->position.y() == at.y()) {
			unit->selected = !unit->selected;
			return unit;
		}
	return nullptr;
}