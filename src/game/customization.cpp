#include "customization.hpp"
#include "skillTree.hpp"
#include "combat.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Customization::Customization(const std::string& file) :
	base("res/assets/UI/UnitBase.png"),
	empty("res/assets/UI/EmptyUnit.png")
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
		generateDefaultUnitData();
	}

	// Initialize the rest of the state
	initSprites();

	ScreenCoord coord1 = ScreenCoord(0 + Core::windowWidth() / 3, 0 + Core::windowHeight() / 5);
	button1 = SkillTreeLinkButton(coord1);
	ScreenCoord coord2 = ScreenCoord(Core::windowWidth() / 2 + Core::windowWidth() / 3, 0 + Core::windowHeight() / 5);
	button2 = SkillTreeLinkButton(coord2);
	ScreenCoord coord3 = ScreenCoord(0 + Core::windowWidth() / 3, Core::windowHeight() / 2 + Core::windowHeight() / 5);
	button3 = SkillTreeLinkButton(coord3);
	ScreenCoord coord4 = ScreenCoord(Core::windowWidth() / 2 + Core::windowWidth() / 3, Core::windowHeight() / 2 + Core::windowHeight() / 5);
	button4 = SkillTreeLinkButton(coord4);
	
}

Customization::~Customization() {

}

// Only generates dummy data right now
// Deprecated...
void Customization::generateDefaultUnitData() {

	// This should never really be the case, but add it as backup anyways
	// TODO: Generate a single default unit
	UnitData defaultUnit;
	defaultUnit.name = "BOB";
	defaultUnit.strength = 10;
	defaultUnit.dexterity = 10;
	defaultUnit.constitution = 10;
	defaultUnit.intelligence = 10;
	units.push_back(defaultUnit);

}

void Customization::initSprites() {
	base.setSize(Core::windowWidth() / 2, Core::windowHeight() / 2);
	empty.setSize(Core::windowWidth() / 2, Core::windowHeight() / 2);

	int width = Core::getTexture("res/assets/UI/Continue.png")->getWidth();
	int height = Core::getTexture("res/assets/UI/Continue.png")->getHeight();
	continueButton = ButtonData(ScreenCoord((Core::windowWidth() - width) / 2, 0), width, height);
	continueButton.setSprites("res/assets/UI/Continue.png", "res/assets/UI/ContinueHover.png", "res/assets/UI/ContinueHover.png");
}

void Customization::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_MOUSEBUTTONUP) {
		Vec2<int> mousePos;
		SDL_GetMouseState(&mousePos.x(), &mousePos.y());
		if (button1.colliding(mousePos)) changeState(new SkillTree(0));
		if (button2.colliding(mousePos)) changeState(new SkillTree(1));
		if (button3.colliding(mousePos)) changeState(new SkillTree(2));
		if (button4.colliding(mousePos)) changeState(new SkillTree(3));
		if (continueButton.colliding(mousePos)) switchToCombatState();
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
	unitSprite.setSize(Core::windowWidth()/6, Core::windowHeight()/3);
	unitSprite.setPos(x+margin+(margin/2), y+margin+(margin/2));
	unitSprite.render();

	// Unit name
	int x_offset = 80;
	Core::Text_Renderer::setColour(Colour(0,0,0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render(unit.name, ScreenCoord(x + (Core::windowWidth()/4) - x_offset, y), 3.f);

	// Unit level
	Core::Text_Renderer::setColour(Colour(0, 0, 0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
	Core::Text_Renderer::render(std::string("LVL. ") + std::to_string(unit.level), ScreenCoord(x + static_cast<int>(Core::windowWidth() / 2.5), y + Core::windowHeight() / 11));

	// Skillls
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	Core::Text_Renderer::render(unit.attack1, ScreenCoord(x + (Core::windowWidth() / 5), y + (Core::windowHeight() / 5)), 1.f);
	Core::Text_Renderer::render(unit.attack2, ScreenCoord(x + (Core::windowWidth() / 5), y + (Core::windowHeight() / 5) + 30), 1.f);
	Core::Text_Renderer::render(unit.attack3, ScreenCoord(x + (Core::windowWidth() / 5), y + (Core::windowHeight() / 5) + 60), 1.f);
	Core::Text_Renderer::render(unit.attack4, ScreenCoord(x + (Core::windowWidth() / 5), y + (Core::windowHeight() / 5) + 90), 1.f);

	// EXP BAR
	// NOTE: assumes that just half the screen height and half the screen width is being used as dimensions
	int height = 10;
	int width = Core::windowWidth() / 4;
	int left_offset = 80;
	ScreenCoord start = ScreenCoord(x + Core::windowWidth() / 4 - left_offset, y + (Core::windowHeight() / 2) / 3 - height);
	Core::Renderer::drawRect(start, width, height, Colour(.7f, .7f, .7f));
	// Draw the actual bar
	int right = lerp(0, width, static_cast<float>(unit.experience) / DEFAULT_MAX_EXP);
	Core::Renderer::drawRect(start, right, height, Colour(.6f, .6f, 1.f));

	// Attributes
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	Core::Text_Renderer::render("STR: " + std::to_string(unit.strength), ScreenCoord(x+(Core::windowWidth()/11), y+(Core::windowHeight()/2) - 20), 1.f);
	Core::Text_Renderer::render("DEX: " + std::to_string(unit.dexterity), ScreenCoord(x+(Core::windowWidth()/11) * 2, y+(Core::windowHeight()/2) - 20), 1.f);
	Core::Text_Renderer::render("INT: " + std::to_string(unit.intelligence), ScreenCoord(x+(Core::windowWidth()/11) * 3, y+(Core::windowHeight()/2) - 20), 1.f);
	Core::Text_Renderer::render("CON: " + std::to_string(unit.constitution), ScreenCoord(x+(Core::windowWidth()/11) * 4, y+(Core::windowHeight()/2) - 20), 1.f);
}

void Customization::renderEmpty(int x, int y) {
	empty.setPos(x, y);
	empty.render();
}

void Customization::render() {

	// Render the units to the screen
	if (units.size() > 0) renderUnit(0, 0, units[0]);
	else (renderEmpty(0, 0));
	if (units.size() > 1) renderUnit(Core::windowWidth()/2, 0, units[1]);
	else (renderEmpty(Core::windowWidth() / 2, 0));
	if (units.size() > 2) renderUnit(0, Core::windowHeight()/2, units[2]);
	else (renderEmpty(0, Core::windowHeight() / 2));
	if (units.size() > 3) renderUnit(Core::windowWidth()/2, Core::windowHeight()/2, units[3]);
	else (renderEmpty(Core::windowWidth() / 2, Core::windowHeight() / 2));

	if (units.size() > 0) button1.render();
	if (units.size() > 1) button2.render();
	if (units.size() > 2) button3.render();
	if (units.size() > 3) button4.render();

	continueButton.render();
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

void Customization::switchToCombatState() {
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
	for (const json& level : masterData["levels"]) {
		if (level["id"] == level_id) {
			// TOOD: not sure if this swap is necessary, but I think the code breaks otherwise
			std::string name = level["file"];
			combatLevelLocation = std::string("res/data/levels/") + name;
		}
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