#include "menu.hpp"
#include "settingsmenu.hpp"
#include "combat.hpp"
#include "creditsmenu.hpp"

Menu::Menu() :
	background("res/assets/menu/background.png"),
	highlight("res/assets/menu/blur.png")
{
	background.setSize(Core::windowWidth(), Core::windowHeight());
	highlight.setSize(Core::windowWidth() / 2, 200);

	buttons.push_back("PLAY GAME");
	buttons.push_back("SETTINGS");
	buttons.push_back("CREDITS");
	buttons.push_back("EXIT");

	buttonCoords.emplace_back(Core::windowWidth() - 150, Core::windowHeight() / 2 + 20);
	buttonCoords.emplace_back(Core::windowWidth() - 150, Core::windowHeight() / 2 + 20 + 60);
	buttonCoords.emplace_back(Core::windowWidth() - 150, Core::windowHeight() / 2 + 20 + 120);
	buttonCoords.emplace_back(Core::windowWidth() - 150, Core::windowHeight() / 2 + 20 + 180);

	selected_option = 0;
}

Menu::~Menu() {

}

void Menu::handleEvent(const SDL_Event & e) {
	if (e.type == SDL_KEYDOWN) {
		// Change the selected option on up/down key presses
		if (e.key.keysym.sym == SDLK_DOWN) {
			selected_option++;
			if (selected_option >= NUM_BUTTONS) selected_option = NUM_BUTTONS - 1;
		}
		if (e.key.keysym.sym == SDLK_UP) {
			selected_option--;
			if (selected_option < 0) selected_option = 0;
		}
		if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
			switch (selected_option) {
			case 0: {
				changeState(new Combat("res/data/levels/level1.json"));
				// Set the text rendering colour back to normal
				Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
			} break;
			case 1: {
				changeState(new SettingsMenu());
			} break;
			case 2: {
				changeState(new CreditsMenu());
			} break;
			case 3: {
				exit(0);
			} break;
			default: {
				exit(1);
			} break;
			}
		}
	}
}

void Menu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
}

void Menu::render() {
	background.render();
	/*
	if (render_text) {
		ScreenCoord pos(380, 450);
		Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		Core::Text_Renderer::render("Press any key to begin", pos, 2.f);
	}
	*/

	// Render the version
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	ScreenCoord version_pos(10, Core::windowHeight() - 40);
	Core::Text_Renderer::setColour(Colour(.6f, 1.f, 1.f));
	Core::Text_Renderer::render(std::string("VERSION ") + VERSION, version_pos, 1.f);

	// Render the highlight sprite
	ScreenCoord coord = buttonCoords[selected_option];
	coord.x() -= Core::windowWidth() / 2 - 100;
	coord.y() -= 55;
	highlight.setPos(coord.x(), coord.y());
	highlight.render();

	// Render the buttons
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::right, TextRenderer::vAlign::top);
	for (int i = 0; i < NUM_BUTTONS; ++i) {
		if (i == selected_option) {
			Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		} else {
			Core::Text_Renderer::setColour(Colour(.6f, 1.f, 1.f));
		}
		Core::Text_Renderer::render(buttons[i], buttonCoords[i], 2.f);
	}
}
