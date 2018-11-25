//STILL NEEDS TO INCLUDE LINK TO SKILL TREE 
//ALSO NEEDS TO READ UNIT DATA FROM A FILE / GLOBAL VARIABLE


#include "customization.hpp"
#include "skillTree.hpp"

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
			int STR = unit["STR"];
			int DEX = unit["DEX"];
			int INT = unit["INT"];
			int CON = unit["CON"];
			units.push_back(UnitData{ name, STR, DEX, INT, CON });
		}
	} else {
		// generate data for units if not found
		// TOOD: Save to a file
		generateDefaultUnitData();
	}

	// Initialize the rest of the state
	initSprites();

	ScreenCoord coord1 = ScreenCoord(0 + Core::windowWidth() / 3, 0 + Core::windowHeight() / 5);
	button1 = ButtonData(coord1);
	ScreenCoord coord2 = ScreenCoord(Core::windowWidth() / 2 + Core::windowWidth() / 3, 0 + Core::windowHeight() / 5);
	button2 = ButtonData(coord2);
	ScreenCoord coord3 = ScreenCoord(0 + Core::windowWidth() / 3, Core::windowHeight() / 2 + Core::windowHeight() / 5);
	button3 = ButtonData(coord3);
	ScreenCoord coord4 = ScreenCoord(Core::windowWidth() / 2 + Core::windowWidth() / 3, Core::windowHeight() / 2 + Core::windowHeight() / 5);
	button4 = ButtonData(coord4);
	
}

Customization::~Customization() {

}

// Only generates dummy data right now
// Deprecated...
void Customization::generateDefaultUnitData() {

	// TODO: Generate a single default unit

	/*
	// Default attributes for each unit1
	unit1.name = "BOB";unit1.strength = 10;unit1.dexterity = 3;unit1.intelligence = 4;unit1.constitution = 100;
	unit2.name = "SAM";unit2.strength = 1;unit2.dexterity = 3;unit2.intelligence = 4;unit2.constitution = 100;
	unit3.name = "TIM";unit3.strength = 10;unit3.dexterity = 3;unit3.intelligence = 4;unit3.constitution = 100;
	unit4.name = "MR. CHERRY";unit4.strength = 40;unit4.dexterity = 3;unit4.intelligence = 4;unit4.constitution = 100;
	*/
}

void Customization::initSprites() {
	base.setSize(Core::windowWidth() / 2, Core::windowHeight() / 2);
	empty.setSize(Core::windowWidth() / 2, Core::windowHeight() / 2);
}

void Customization::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_MOUSEBUTTONUP) {
		Vec2<int> mousePos;
		SDL_GetMouseState(&mousePos.x(), &mousePos.y());
		if (button1.colliding(mousePos)) changeState(new SkillTree(0));
		if (button2.colliding(mousePos)) changeState(new SkillTree(1));
		if (button3.colliding(mousePos)) changeState(new SkillTree(2));
		if (button4.colliding(mousePos)) changeState(new SkillTree(3));
	}
}

void Customization::update(int delta) {

}
//renders Unit data where x,y is the left top corner of the unit data box and unit is the unit to be render
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

	//unit name
	Core::Text_Renderer::setColour(Colour(0,0,0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
    Core::Text_Renderer::render(unit.name, ScreenCoord(x+(Core::windowWidth()/4), y), 3.f);

	//attributes
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Strength: " + std::to_string(unit.strength), ScreenCoord(x+(Core::windowWidth()/5), y+(Core::windowHeight()/5)), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.dexterity), ScreenCoord(x+(Core::windowWidth()/5), y+(Core::windowHeight()/5)+30), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.intelligence), ScreenCoord(x+(Core::windowWidth()/5), y+(Core::windowHeight()/5)+60), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.constitution), ScreenCoord(x+(Core::windowWidth()/5), y+(Core::windowHeight()/5)+90), 1.f);
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

}

void Customization::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}

ButtonData::ButtonData(Vec2<int> position, int width, int height) :
	default_sprite("res/assets/UI/SkillTreeLink.png"),
	position(position),
	width(width),
	height(height)
{
	default_sprite.setSize(width, height);
	default_sprite.setPos(position.x(), position.y());
}

ButtonData::~ButtonData() {

}

void ButtonData::render() {
	default_sprite.render();
	Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::middle);
	// TODO: calculate offset from window width/height somehow
	Core::Text_Renderer::render("Skill Tree", position + Vec2<int>(25, 35), 1.0);
}

bool ButtonData::colliding(ScreenCoord point) {
	if (point.x() >= position.x() && point.x() <= position.x() + width &&
		point.y() >= position.y() && point.y() <= position.y() + height) 
	{
		return true;
	}
	return false;
}
