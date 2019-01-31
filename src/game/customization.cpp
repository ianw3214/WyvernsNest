#include "customization.hpp"
#include "skillTree.hpp"
#include "combat.hpp"
#include "util/util.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

Customization::Customization(const std::string& file) :
	base("res/assets/UI/UnitBase.png"),
	empty("res/assets/UI/EmptyUnit.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png")
{
	std::ifstream player_file(file);
	if (player_file.is_open()) {
		json data;
		player_file >> data;
		for (const json& unit : data["players"]) {
			std::string name = unit["name"];
			int STR					= unit["STR"];
			int DEX					= unit["DEX"];
			int INT					= unit["INT"];
			int CON					= unit["CON"];
			int level				= unit["level"];
			int experience			= unit["experience"];
			std::vector<Trait> traits;	// TODO
			std::vector<int> tree	= unit["selected"];
			std::string attack1 = unit["attack1"];
			std::string attack2 = unit["attack2"];
			std::string attack3 = unit["attack3"];
			std::string attack4 = unit["attack4"];
			units.push_back(UnitData{ name, STR, DEX, INT, CON, level, experience, traits, tree, attack1, attack2, attack3, attack4 });
		}
		initAvailableAttacks();
	} else {
		// This should actually never run, perhaps delete the save and force exit the game
		// generate data for units if not found
		// TOOD: Save to a file
	}

	// Initialize the rest of the state
	initSprites();

	ScreenCoord coord1 = ScreenCoord(SubDiv::hPos(6, 2), SubDiv::vPos(10, 2));
	button1 = SkillTreeLinkButton(coord1);
	ScreenCoord coord2 = ScreenCoord(SubDiv::hPos(6, 5), SubDiv::vPos(10, 2));
	button2 = SkillTreeLinkButton(coord2);
	ScreenCoord coord3 = ScreenCoord(SubDiv::hPos(6, 2), SubDiv::vPos(10, 7));
	button3 = SkillTreeLinkButton(coord3);
	ScreenCoord coord4 = ScreenCoord(SubDiv::hPos(6, 5), SubDiv::vPos(10, 7));
	button4 = SkillTreeLinkButton(coord4);

	// Initialize skill cycle buttons
	for (int i = 0; i < 4; ++i) {
		int x = 0;
		int y = 0;
		if (i == 1 || i == 3) x = SubDiv::hCenter();
		if (i == 2 || i == 3) y = SubDiv::vCenter();

		// Left side
		ScreenCoord skillCycleButton1Coord = ScreenCoord(x + SubDiv::hPos(5, 1) - 40, y + SubDiv::vPos(5, 1) + 4);
		ScreenCoord skillCycleButton2Coord = ScreenCoord(x + SubDiv::hPos(5, 1) - 40, y + SubDiv::vPos(5, 1) + 40);
		ScreenCoord skillCycleButton3Coord = ScreenCoord(x + SubDiv::hPos(5, 1) - 40, y + SubDiv::vPos(5, 1) + 76);
		ScreenCoord skillCycleButton4Coord = ScreenCoord(x + SubDiv::hPos(5, 1) - 40, y + SubDiv::vPos(5, 1) + 112);
		// Right side
		ScreenCoord skillCycleButton5Coord = ScreenCoord(x + SubDiv::hPos(5, 1) + 146, y + SubDiv::vPos(5, 1) + 4);
		ScreenCoord skillCycleButton6Coord = ScreenCoord(x + SubDiv::hPos(5, 1) + 146, y + SubDiv::vPos(5, 1) + 40);
		ScreenCoord skillCycleButton7Coord = ScreenCoord(x + SubDiv::hPos(5, 1) + 146, y + SubDiv::vPos(5, 1) + 76);
		ScreenCoord skillCycleButton8Coord = ScreenCoord(x + SubDiv::hPos(5, 1) + 146, y + SubDiv::vPos(5, 1) + 112);

		skillCycleButtons.emplace_back(skillCycleButton1Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton2Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton3Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton4Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton5Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton6Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton7Coord, 32, 32);
		skillCycleButtons.emplace_back(skillCycleButton8Coord, 32, 32);

		// Set the sprites of the skill cycle buttons
		for (unsigned int i = skillCycleButtons.size() - 8; i < skillCycleButtons.size(); ++i) {
			skillCycleButtons[i].setSprites("res/assets/UI/arrows.png", "res/assets/UI/arrows.png", "res/assets/UI/arrows.png");
			if (i % 8 < 4) {	// LEFT BUTTON
				skillCycleButtons[i].default_sprite.setSourcePos(0, 0);
				skillCycleButtons[i].hover_sprite.setSourcePos(64, 0);
				skillCycleButtons[i].default_sprite.setSourceSize(32, 32);
				skillCycleButtons[i].hover_sprite.setSourceSize(32, 32);
			} else {			// RIGHT BUTTON
				skillCycleButtons[i].default_sprite.setSourcePos(32, 0);
				skillCycleButtons[i].hover_sprite.setSourcePos(96, 0);
				skillCycleButtons[i].default_sprite.setSourceSize(32, 32);
				skillCycleButtons[i].hover_sprite.setSourceSize(32, 32);
			}
		}
	}

}

Customization::~Customization() {

}

void Customization::initSprites() {
	base.setSize(SubDiv::hSize(2, 1), SubDiv::vSize(2, 1));
	empty.setSize(SubDiv::hSize(2, 1), SubDiv::vSize(2, 1));

	int width = Core::getTexture("res/assets/UI/Continue.png")->getWidth();
	int height = Core::getTexture("res/assets/UI/Continue.png")->getHeight();
	continueButton = ButtonData(ScreenCoord(SubDiv::hCenter() - width / 2, 0), width, height);
	continueButton.setSprites("res/assets/UI/Continue.png", "res/assets/UI/ContinueHover.png", "res/assets/UI/ContinueHover.png");
}

void Customization::handleEvent(const SDL_Event& e) {
	Vec2<int> mousePos;
	SDL_GetMouseState(&mousePos.x(), &mousePos.y());
	if (e.type == SDL_MOUSEBUTTONUP) {
		if (units.size() > 0 && button1.colliding(mousePos)) {
			saveData();
			changeState(new SkillTree(0));
		}
		if (units.size() > 1 && button2.colliding(mousePos)) {
			saveData();
			changeState(new SkillTree(1));
		}
		if (units.size() > 2 && button3.colliding(mousePos)) {
			saveData();
			changeState(new SkillTree(2));
		}
		if (units.size() > 3 && button4.colliding(mousePos)) {
			saveData();
			changeState(new SkillTree(3));
		}
		if (continueButton.colliding(mousePos)) switchToCombatState();
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		// Render skill cycle buttons
		for (unsigned int i = 0; i < 8 * units.size(); ++i) {
			if (skillCycleButtons[i].colliding(mousePos)) {
				// First, figure out which unit's button is being clicked
				int unit = i / 8;
				// Also, figure out which button of the unit is being clicked
				int button = i % 8;
				// From the clicked button, figure out which attack is being changed
				int i_attack = button % 4;
				// Also, figure out the direction that the attack is being changed in
				bool forward = button >= 4;
				// If there are attacks available to change, change it
				if (attacks[unit].size() > 4) {
					cycleAttack(unit, button, i_attack, forward);
				}
			}
		}
	}
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
			switchToCombatState();
		}
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			exit(0);
		}
	}
	// Update the mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
	}
}

void Customization::update(int delta) {
	
}

// Renders Unit data where x,y is the left top corner of the unit data box and unit is the unit to be render
void Customization::renderUnit(int x, int y, UnitData unit){
	// Render the base sprite first
	base.setPos(x, y);
	base.render();

	int margin = 10;

	//draw unit sprite
	Sprite unitSprite("res/assets/players/MaleBase.png");
	unitSprite.setSize(SubDiv::hSize(6, 1), SubDiv::vSize(3, 1));
	unitSprite.setPos(x+margin+(margin/2), y+margin+(margin/2));
	unitSprite.render();

	// Unit name
	int x_offset = 80;
	Core::Text_Renderer::setColour(Colour(0,0,0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render(unit.name, ScreenCoord(x + SubDiv::hSize(4, 1) - x_offset, y), 3.f);

	// Unit level
	Core::Text_Renderer::setColour(Colour(0, 0, 0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
	Core::Text_Renderer::render(std::string("LVL. ") + std::to_string(unit.level), ScreenCoord(x + static_cast<int>(Core::windowWidth() / 2.5f), y + Core::windowHeight() / 11));

	// Skills
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	Core::Text_Renderer::render(unit.attack1, ScreenCoord(x + SubDiv::hPos(5, 1), y + SubDiv::vPos(5, 1)), 1.f);
	Core::Text_Renderer::render(unit.attack2, ScreenCoord(x + SubDiv::hPos(5, 1), y + SubDiv::vPos(5, 1) + 36), 1.f);
	Core::Text_Renderer::render(unit.attack3, ScreenCoord(x + SubDiv::hPos(5, 1), y + SubDiv::vPos(5, 1) + 72), 1.f);
	Core::Text_Renderer::render(unit.attack4, ScreenCoord(x + SubDiv::hPos(5, 1), y + SubDiv::vPos(5, 1) + 108), 1.f);

	// if mouse button clicked
	Vec2<int> mousePos;
	SDL_GetMouseState(&mousePos.x(), &mousePos.y());

	// EXP BAR
	// NOTE: assumes that just half the screen height and half the screen width is being used as dimensions
	int height = 10;
	int width = SubDiv::hSize(4, 1);
	int left_offset = 80;
	ScreenCoord start = ScreenCoord(x + SubDiv::hSize(4, 1) - left_offset, y + SubDiv::vSize(6, 1) - height);
	Core::Renderer::drawRect(start, width, height, Colour(.7f, .7f, .7f));
	// Draw the actual bar
	int right = lerp(0, width, static_cast<float>(unit.experience) / DEFAULT_MAX_EXP);
	Core::Renderer::drawRect(start, right, height, Colour(.6f, .6f, 1.f));

	// Attributes
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::bottom);
	Core::Text_Renderer::render("STR: " + std::to_string(unit.strength), ScreenCoord(x+ SubDiv::hSize(10, 1), y + SubDiv::vSize(2, 1) - 20), 1.f);
	Core::Text_Renderer::render("DEX: " + std::to_string(unit.dexterity), ScreenCoord(x+ SubDiv::hSize(10, 2), y+ SubDiv::vSize(2, 1) - 20), 1.f);
	Core::Text_Renderer::render("INT: " + std::to_string(unit.intelligence), ScreenCoord(x+ SubDiv::hSize(10, 3), y+ SubDiv::vSize(2, 1) - 20), 1.f);
	Core::Text_Renderer::render("CON: " + std::to_string(unit.constitution), ScreenCoord(x+ SubDiv::hSize(10, 4), y+ SubDiv::vSize(2, 1) - 20), 1.f);
}

void Customization::renderEmpty(int x, int y) {
	empty.setPos(x, y);
	empty.render();
}

void Customization::render() {

	// Render the units to the screen
	if (units.size() > 0) renderUnit(0, 0, units[0]);
	else (renderEmpty(0, 0));
	if (units.size() > 1) renderUnit(SubDiv::hCenter(), 0, units[1]);
	else (renderEmpty(SubDiv::hCenter(), 0));
	if (units.size() > 2) renderUnit(0, SubDiv::vCenter(), units[2]);
	else (renderEmpty(0, SubDiv::vCenter()));
	if (units.size() > 3) renderUnit(SubDiv::hCenter(), SubDiv::vCenter(), units[3]);
	else (renderEmpty(SubDiv::hCenter(), SubDiv::vCenter()));

	// Render skill tree link buttons
	if (units.size() > 0) button1.render();
	if (units.size() > 1) button2.render();
	if (units.size() > 2) button3.render();
	if (units.size() > 3) button4.render();

	// Render skill cycle buttons
	for (unsigned int i = 0; i < 8 * units.size(); ++i) {
		skillCycleButtons[i].render();
	}
	
	continueButton.render();

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	} else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void Customization::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}

SkillTreeLinkButton::SkillTreeLinkButton(Vec2<int> position, int width, int height) : 
	ButtonData(position, width, height) {}

void SkillTreeLinkButton::render() {
	ButtonData::render();
	Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::middle);
	// TODO: calculate offset from window width/height somehow
	Core::Text_Renderer::render("Skill Tree", position + Vec2<int>(25, 35), 1.0);
}

void Customization::saveData() {
	json save_data;
	std::ifstream in_save_file(DEFAULT_PLAYER_FILE);
	in_save_file >> save_data;
	in_save_file.close();

	// Update changes in units
	int index = 0;
	for (json& unit : save_data["players"]) {
		unit["attack1"] = units[index].attack1;
		unit["attack2"] = units[index].attack2;
		unit["attack3"] = units[index].attack3;
		unit["attack4"] = units[index].attack4;
		index++;
	}

	// Save it to the save file
	std::ofstream out_save_file(DEFAULT_PLAYER_FILE);
	out_save_file << save_data.dump(4);
	out_save_file.close();
}

void Customization::switchToCombatState() {
	saveData();
	// Assume the save file is always valid
	std::ifstream save_file(DEFAULT_PLAYER_FILE);
	json inputData;
	save_file >> inputData;
	int level_id = inputData["level"];
	// ADD ONE TO THE LEVEL TO ADVANCE TO THE NEXT LEVEL
	level_id++;
	// Change the state based on the level file
	std::string combatLevelLocation;
	std::ifstream masterFile(DEFAULT_MASTER_FILE);
	json masterData;
	masterFile >> masterData;
	// TODO: Temporary, change this in the future
	bool level_found = false;
	for (const json& level : masterData["levels"]) {
		if (level["id"] == level_id) {
			// TOOD: not sure if this swap is necessary, but I think the code breaks otherwise
			std::string name = level["file"];
			combatLevelLocation = std::string("res/data/levels/") + name;
			level_found = true;
		}
		// Temporary debugging code
		// TODO: remove this
		if (!level_found) {
			std::string name = level["file"];
			combatLevelLocation = std::string("res/data/levels/") + name;
		}
	}
	save_file.close();
	masterFile.close();
	if (level_found) {
		// Save the new level to the game
		inputData["level"] = level_id;
		std::ofstream out_save_file(DEFAULT_PLAYER_FILE);
		out_save_file << inputData.dump(4);
		out_save_file.close();
	}
	changeState(new Combat(combatLevelLocation)); 
}

void Customization::initAvailableAttacks() {
	std::ifstream attacks_file(DEFAULT_ATTACK_FILE);
	json attacks_data;
	attacks_file >> attacks_data;

	for (unsigned int i = 0; i < units.size(); ++i) {
		attacks.push_back(std::vector<std::string>());
		for (int j : units[i].skillTree) {
			for (const json& attack : attacks_data["attacks"]) {
				if (attack.find("nodeid") != attack.end() && attack["nodeid"] == j) {
					attacks[i].push_back(attack["name"]);
				}
			}
		}
	}
}

void Customization::cycleAttack(int unit, int button, int i_attack, bool forward) {
	std::string current_attack;
	if (i_attack == 0) current_attack = units[unit].attack1;
	if (i_attack == 1) current_attack = units[unit].attack2;
	if (i_attack == 2) current_attack = units[unit].attack3;
	if (i_attack == 3) current_attack = units[unit].attack4;
	// First, figure out the index of the current attack
	int index = -1;
	for (unsigned int i = 0; i < attacks[unit].size(); ++i) {
		if (attacks[unit][i] == current_attack) {
			index = i;
		}
	}
	if (index < 0) return;
	// Then, cycle through the vector
	int i = forward ? index + 1 : index - 1;
	if (i < 0) i = attacks[unit].size() - 1;
	if (i >= static_cast<int>(attacks[unit].size())) i = 0;
	while (i != index) {
		const std::string& cur = attacks[unit][i];
		// Check if the attack is already a unit attack, otherwise change it
		if (cur != units[unit].attack1 && cur != units[unit].attack2 &&
			cur != units[unit].attack3 && cur != units[unit].attack4)
		{
			if (i_attack == 0) units[unit].attack1 = cur;
			if (i_attack == 1) units[unit].attack2 = cur;
			if (i_attack == 2) units[unit].attack3 = cur;
			if (i_attack == 3) units[unit].attack4 = cur;
		}
		i += forward ? 1 : -1;
		if (i < 0) i = attacks[unit].size() - 1;
		if (i >= static_cast<int>(attacks[unit].size())) i = 0;
	}
}
