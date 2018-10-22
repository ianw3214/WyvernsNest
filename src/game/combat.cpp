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
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (grid.isMousePosValid()) {


			int previous = selectedInt;

			selectedInt = getIndexAt(grid.mousePos);


			if (selectedInt != -1 && selectedInt != previous && previous != -1) {
				((Player *)entities[previous])->selected = false;
				//selectedInt = -1;
			}
			else if (selectedInt == -1 && previous != -1) {
				((Player *)entities[previous])->move(grid.mousePos);
				((Player *)entities[previous])->selected = false;
			}

			//if (selectedPlayer != nullptr) {
			//	selectedPlayer->selected = false;
			//	selectedPlayer = nullptr;
			//}
			//selectedPlayer = &(getPlayerAt(grid.mousePos));

		}		
	}
}

void Combat::update(int delta) {
	// Update shit here
	grid.update();
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
