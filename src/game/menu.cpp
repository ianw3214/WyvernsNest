#include "menu.hpp"

#include "combat.hpp"

Menu::Menu() : background("res/assets/menu/background.png") {
	background.setSize(Core::windowWidth(), Core::windowHeight());
}

Menu::~Menu() {

}

void Menu::handleEvent(const SDL_Event & e) {
	if (e.type == SDL_KEYDOWN) {
		// Move to the combat state upon ANY key press
		changeState(new Combat());
		// Set the text rendering colour back to normal
		Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
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
