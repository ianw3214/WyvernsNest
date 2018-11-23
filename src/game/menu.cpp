#include "menu.hpp"
#include "settingsmenu.hpp"
#include "combat.hpp"
#include "creditsmenu.hpp"

Menu::Menu() : background("res/assets/menu/background.png") {
	background.setSize(Core::windowWidth(), Core::windowHeight());
}

Menu::~Menu() {

}

void Menu::handleEvent(const SDL_Event & e) {
	if (e.type == SDL_KEYDOWN) {
		// Move to the combat state upon A-key press
		if (e.key.keysym.sym == SDLK_a) {
			changeState(new Combat());
			// Set the text rendering colour back to normal
			Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
		}
		// Move to settings menu state upon S-key press
		else if (e.key.keysym.sym == SDLK_s) {
			changeState(new SettingsMenu());
		}
		// Move to credits menu state upon D-key press
		else if (e.key.keysym.sym == SDLK_d) {
			changeState(new CreditsMenu());
		}
	}
}

void Menu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
}

void Menu::render() {
	background.render();
	if (render_text) {
		ScreenCoord pos(380, 450);
		Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		Core::Text_Renderer::render("Press any key to begin", pos, 2.f);
	}
}
