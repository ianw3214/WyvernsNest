#include "settingsmenu.hpp"
#include "menu.hpp"

SettingsMenu::SettingsMenu() : background("res/assets/menu/settingsmenu.png") {
	background.setSize(Core::windowWidth(), Core::windowHeight());
}


SettingsMenu::~SettingsMenu() {
}

void SettingsMenu::handleEvent(const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		// Go back to main menu if X-key pressed
		if (e.key.keysym.sym == SDLK_x) {
			changeState(new Menu());
		}
	}
}

void SettingsMenu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
}

void SettingsMenu::render() {
	background.render();
	if (render_text) {
		ScreenCoord pos(380, 450);
		Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		Core::Text_Renderer::render("Press any key to begin", pos, 2.f);
	}
}