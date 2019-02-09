#include "settingsmenu.hpp"
#include "menu.hpp"

#include "../util/util.hpp"

SettingsMenu::SettingsMenu() :
	background("res/assets/menu/base.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png"),
	slider("res/assets/menu/slider.png"),
	aspect("res/assets/menu/aspect.png")
{
	background.setSize(Core::windowWidth(), Core::windowHeight());
	slider.setSize(SubDiv::hSize(3, 1), SubDiv::vSize(12, 1));
	aspect.setSize(SubDiv::hSize(3, 1), SubDiv::vSize(12, 1));

	selected_option = 0;
}


SettingsMenu::~SettingsMenu() {

}

void SettingsMenu::handleEvent(const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		// Change the selected option on up/down key presses
		if (e.key.keysym.sym == SDLK_DOWN) {
			selected_option++;
			if (selected_option >= NUM_OPTIONS) selected_option = NUM_OPTIONS - 1;
		}
		if (e.key.keysym.sym == SDLK_UP) {
			selected_option--;
			if (selected_option < 0) selected_option = 0;
		}
		if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
			if (selected_option == 2) changeState(new Menu(false));
		}
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			if (selected_option == 2) changeState(new Menu(false));
			else selected_option = 2;
		}
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
		// TODO: figure this shit out
		if (selected_option == 2) changeState(new Menu(false));
	}
}

void SettingsMenu::update(int delta) {
	// Update the currently selected if the mouse is hovering over it
	SDL_GetMouseState(&mouseX, &mouseY);
	// mouse over the volume option
	if (mouseX > SubDiv::hPos(3, 1) && mouseX < SubDiv::hPos(3, 1) + SubDiv::hSize(6, 1) &&
		mouseY > SubDiv::vPos(20, 10) - SubDiv::vSize(12, 1) && mouseY < SubDiv::vPos(20, 10))
	{
		selected_option = 0;
	}
	// Render the resolution option
	if (mouseX > SubDiv::hPos(3, 1) && mouseX < SubDiv::hPos(3, 1) + SubDiv::hSize(6, 1) &&
		mouseY > SubDiv::vPos(20, 13) - SubDiv::vSize(12, 1) && mouseY < SubDiv::vPos(20, 13))
	{
		selected_option = 1;
	}
	// Render the back option
	if (mouseX > SubDiv::hCenter() - SubDiv::hSize(8, 1) && mouseX < SubDiv::hCenter() + SubDiv::hSize(8, 1) &&
		mouseY > SubDiv::vPos(20, 17) - SubDiv::vSize(18, 1) && mouseY < SubDiv::vPos(20, 17) + SubDiv::vSize(18, 1))
	{
		selected_option = 2;
	}
}

void SettingsMenu::render() {
	background.render();

	// Render the version
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	ScreenCoord version_pos(10, Core::windowHeight() - 10);
	Core::Text_Renderer::setColour(Colour(.6f, 1.f, 1.f));
	Core::Text_Renderer::render(std::string("VERSION ") + VERSION, version_pos, 1.f);

	// Render the settings title
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::bottom);
	ScreenCoord title_pos(SubDiv::hCenter(), SubDiv::vPos(5, 2));
	Core::Text_Renderer::setColour(Colour(.2f, .9f, .9f));
	Core::Text_Renderer::render(TITLE_TEXT, title_pos, 3.5f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::bottom);
	// Render the volume option
	if (selected_option == 0) Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
	else Core::Text_Renderer::setColour(Colour(.2f, .8f, .8f));
	ScreenCoord volume_pos(SubDiv::hPos(3, 1), SubDiv::vPos(20, 10));
	Core::Text_Renderer::render(VOLUME_TEXT, volume_pos, 1.5f);
	slider.setPos(SubDiv::hPos(3, 1), SubDiv::vPos(20, 10));
	slider.render();
	// Render the resolution option
	if (selected_option == 1) Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
	else Core::Text_Renderer::setColour(Colour(.2f, .8f, .8f));
	ScreenCoord res_pos(SubDiv::hPos(3, 1), SubDiv::vPos(20, 13));
	Core::Text_Renderer::render(RES_TEXT, res_pos, 1.5f);
	aspect.setPos(SubDiv::hPos(3, 1), SubDiv::vPos(20, 13));
	aspect.render();
	// Render the back option
	if (selected_option == 2) Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
	else Core::Text_Renderer::setColour(Colour(.2f, .9f, .9f));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::bottom);
	ScreenCoord back_pos(SubDiv::hCenter(), SubDiv::vPos(20, 17));
	Core::Text_Renderer::render(BACK_TEXT, back_pos, 1.5f);


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