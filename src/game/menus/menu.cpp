#include "menu.hpp"
#include "../util/util.hpp"
#include "settingsmenu.hpp"
#include "creditsmenu.hpp"
#include "../combat.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Menu::Menu() :
	background("res/assets/menu/background.png"),
	highlight("res/assets/menu/blur.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png")
{
	background.setSize(Core::windowWidth(), Core::windowHeight());
	highlight.setSize(Core::windowWidth() / 2, 200);

	buttons.push_back("PLAY GAME");
	buttons.push_back("SETTINGS");
	buttons.push_back("CREDITS");
	buttons.push_back("EXIT");

	buttonCoords.emplace_back(SubDiv::hPos(10, 9), SubDiv::vPos(10, 5));
	buttonCoords.emplace_back(SubDiv::hPos(10, 9), SubDiv::vPos(10, 6));
	buttonCoords.emplace_back(SubDiv::hPos(10, 9), SubDiv::vPos(10, 7));
	buttonCoords.emplace_back(SubDiv::hPos(10, 9), SubDiv::vPos(10, 8));

	selected_option = 0;

	SDL_ShowCursor(SDL_DISABLE);
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

void Menu::update(int delta) {
	counter++;
	if (counter % 20 == 0) render_text = !render_text;
	// Update the currently selected if the mouse is hovering over it
	SDL_GetMouseState(&mouseX, &mouseY);
	if (getButtonIndexAtPos(ScreenCoord(mouseX, mouseY)) >= 0) selected_option = getButtonIndexAtPos(ScreenCoord(mouseX, mouseY));
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
	for (int i = 0; i < NUM_BUTTONS; ++i) {
		if (i == selected_option) {
			Core::Text_Renderer::setColour(Colour(1.f, 1.f, 1.f));
		} else {
			Core::Text_Renderer::setColour(Colour(.6f, 1.f, 1.f));
		}
		Core::Text_Renderer::render(buttons[i], buttonCoords[i], 2.f);
	}

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	} else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void Menu::switchToCurrentState() {
	switch (selected_option) {
	case 0: {
		changeToCombatState();
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

void Menu::changeToCombatState() {
	int level_id = 1;
	std::ifstream save_file(USER_SAVE_LOCATION);
	// Create a new save file for the user if it doesn't exist
	if (!save_file.is_open()) {
		initializeSaveFile();
	}
	// Otherwise, validate the save file first
	else {
		std::ifstream old_save(USER_SAVE_LOCATION);
		json inputData;
		old_save >> inputData;
		bool valid = true;
		if (inputData.find("players") == inputData.end()) valid = false;
		if (inputData.find("level") == inputData.end()) valid = false;
		// Generate a new save file if the current one is corrupted
		if (!valid) {
			save_file.close();
			initializeSaveFile();
			}
		else level_id = inputData["level"];
	}
	// Change the state based on the level file
	std::string combatLevelLocation;
	std::ifstream masterFile(MASTER_LEVEL_LOCATION);
	json masterData;
	masterFile >> masterData;
	for (const json& level : masterData["levels"]) {
		if (level["id"] == level_id) {
			// TOOD: not sure if this swap is necessary, but I think the code breaks otherwise
			std::string name = level["file"];
			combatLevelLocation = std::string("res/data/levels/") + name;
		}
	}

	changeState(new Combat(combatLevelLocation));

	// Set the text rendering colour back to normal
	Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
	SDL_ShowCursor(SDL_ENABLE);
}

void Menu::initializeSaveFile() {
	std::ofstream new_save(USER_SAVE_LOCATION);
	json outputData;
	std::vector<json> initialPlayers;
	json player;
	player["STR"] = 10;
	player["DEX"] = 10;
	player["INT"] = 10;
	player["CON"] = 10;
	player["experience"] = 0;
	player["level"] = 1;
	// TODO: Randomize this
	player["name"] = "DEFAULT";
	std::vector<int> selected;
	selected.push_back(1);
	player["selected"] = selected;
	// Initialize player attacks
	std::vector<std::string> attacks;
	player["attack1"] = DEFAULT_PLAYER_ATTACK_1;
	player["attack2"] = DEFAULT_PLAYER_ATTACK_2;
	player["attack3"] = DEFAULT_PLAYER_ATTACK_3;
	player["attack4"] = DEFAULT_PLAYER_ATTACK_4;
	initialPlayers.push_back(player);
	// Construct the actual save file data
	outputData["players"] = initialPlayers;
	outputData["level"] = 1;
	// Write the initial data to the save file
	new_save << outputData.dump(4);
}

int Menu::getButtonIndexAtPos(ScreenCoord coord) {
	for (int i = 0; i < NUM_BUTTONS; ++i) {
		int left = buttonCoords[i].x() - SubDiv::hSize(5, 2);
		int right = buttonCoords[i].x();
		int top = buttonCoords[i].y() + 10;
		int bottom = buttonCoords[i].y() + 65;
		if (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom) {
			return i;
		}
	}
	return -1;
}
