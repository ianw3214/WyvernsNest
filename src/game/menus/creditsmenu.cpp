#include "creditsmenu.hpp"
#include "menu.hpp"

CreditsMenu::CreditsMenu() : background("res/assets/menu/creditsmenu.png") {
	background.setSize(Core::windowWidth(), Core::windowHeight());
}


CreditsMenu::~CreditsMenu() {
}

void CreditsMenu::handleEvent(const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		// Go back to main menu if X-key pressed
		if (e.key.keysym.sym == SDLK_x) {
			changeState(new Menu());
		}
	}
}

void CreditsMenu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
}

void CreditsMenu::render() {
	background.render();
	if (render_text) {
		ScreenCoord pos(380, 450);
		Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		Core::Text_Renderer::render("Press any key to begin", pos, 2.f);
	}
}