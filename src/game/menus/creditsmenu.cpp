#include "creditsmenu.hpp"
#include "menu.hpp"

CreditsMenu::CreditsMenu() :
	background("res/assets/menu/creditsmenu.png"),
	highlight("res/assets/menu/blur.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png") 
{
	background.setSize(Core::windowWidth(), Core::windowHeight());
	highlight.setSize(Core::windowWidth() / 2, 200);

	buttons.push_back("BACK");

	buttonCoords.emplace_back(Core::windowWidth() - 150, Core::windowHeight() / 2 + 20 + 180);

	selected_option = 0;

	SDL_ShowCursor(SDL_DISABLE);
}


CreditsMenu::~CreditsMenu() {
	SDL_ShowCursor(SDL_DISABLE);
}

void CreditsMenu::handleEvent(const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		// Change the selected option on up/down key presses
		if (e.key.keysym.sym == SDLK_DOWN) {
			selected_option++;
			if (selected_option >= NUM_BUTTONS_CRD) selected_option = NUM_BUTTONS_CRD - 1;
		}
		if (e.key.keysym.sym == SDLK_UP) {
			selected_option--;
			if (selected_option < 0) selected_option = 0;
		}
		if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
			switchToCurrentState();
		}
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
		if (getButtonIndexAtPos(ScreenCoord(mouseX, mouseY)) >= 0) {
			switchToCurrentState();
		}
	}
}

void CreditsMenu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
	// Update the currently selected if the mouse is hovering over it
	SDL_GetMouseState(&mouseX, &mouseY);
	if (getButtonIndexAtPos(ScreenCoord(mouseX, mouseY)) >= 0) selected_option = getButtonIndexAtPos(ScreenCoord(mouseX, mouseY));
}

void CreditsMenu::render() {
	background.render();

	// Render the version
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	ScreenCoord version_pos(10, Core::windowHeight() - 10);
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
	for (int i = 0; i < NUM_BUTTONS_CRD; ++i) {
		if (i == selected_option) {
			Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		}
		else {
			Core::Text_Renderer::setColour(Colour(.6f, 1.f, 1.f));
		}
		Core::Text_Renderer::render(buttons[i], buttonCoords[i], 2.f);
	}

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	}
	else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void CreditsMenu::switchToCurrentState() {
	switch (selected_option) {
	case 0: {
		changeState(new Menu(false));
	} break;
	default: {
		exit(1);
	} break;
	}
}

int CreditsMenu::getButtonIndexAtPos(ScreenCoord coord) {
	for (int i = 0; i < NUM_BUTTONS_CRD; ++i) {
		int left = buttonCoords[i].x() - Core::windowWidth() / 2 + 100;
		int right = buttonCoords[i].x();
		int top = buttonCoords[i].y() + 10;
		int bottom = buttonCoords[i].y() + 65;
		if (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom) {
			return i;
		}
	}
	return -1;
}