#include "combat.hpp"

Combat::Combat() {

}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {
	//	if (e.type == SDL_KEYDOWN) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
	//	if (e.key.keysym.sym == SDLK_SPACE) {
			// Do something when space is pressed
			//playerTurn = !playerTurn;
			if (grid.isMousePosValid()) {

				if (player.position.x() == grid.mousePos.x() && player.position.y() == grid.mousePos.y()) {
					//selected = player;
					player.selected = true;
				}
				else {
					if (player.selected) {
						player.move(grid.mousePos);
						player.selected = false;
					}
					//if (selected.id != -1) {
					//	selected.selected = false;
						//selected = null;
					//}

				}
			}
		//}
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
		player.render();

	}
	else {
		Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));

	}
	// SAMPLE CODE
}
