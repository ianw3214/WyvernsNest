#include "combat.hpp"

Combat::Combat() {
	//players.push_back(Player());

	player1.setTileSize(grid.tile_width, grid.tile_height);
	player2.setTileSize(grid.tile_width, grid.tile_height);

	entities.push_back(&player1);
	entities.push_back(&player2);

}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {

	if (selectedInt == -1 || (selectedInt != -1 && ((Player *)entities[selectedInt])->state == 0)) {

		for (Entity * entity : entities) entity->handleEvent(e);

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (grid.isMousePosValid()) {


				int previous = selectedInt;

				selectedInt = getIndexAt(grid.mousePos);


				if (selectedInt != -1 && selectedInt != previous && previous != -1) {
					((Player *)entities[previous])->selected = false;
					//TODO turn off attacks
				}
				else if (selectedInt == -1 && previous != -1) {
					ScreenCoord target = ((Player *)entities[previous])->move(grid.mousePos);
					((Player *)entities[previous])->selected = false;

					selectedInt = previous;
					//TODO do something with the target
				}

				//if (selectedPlayer != nullptr) {
				//	selectedPlayer->selected = false;
				//	selectedPlayer = nullptr;
				//}
				//selectedPlayer = &(getPlayerAt(grid.mousePos));

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
	//	player.render();
		//player.render();
		//player2.render();

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
	// SAMPLE CODE


}

Player Combat::getPlayerAt(ScreenCoord at)
{
	int test3 = 0;
	for (Entity * e : entities) {
		if (dynamic_cast<Player*>(e) != nullptr) {
			if (((Player *) e)->position.x() == grid.mousePos.x() && ((Player *)e)->position.y() == grid.mousePos.y()) {
				((Player *) e)->selected = !((Player *)e)->selected;
				return *((Player *)e);
			}
		}
	}
}

int Combat::getIndexAt(ScreenCoord at)
{
	for (int i = 0; i < entities.size(); i ++) {
		Entity *  e = entities[i];
		if (dynamic_cast<Player*>(e) != nullptr) {
			if (((Player *)e)->position.x() == grid.mousePos.x() && ((Player *)e)->position.y() == grid.mousePos.y()) {
				((Player *)e)->selected = !((Player *)e)->selected;
				return i;
			}
		}
	}
	return -1;
}