//STILL NEEDS TO INCLUDE LINK TO SKILL TREE 
//ALSO NEEDS TO READ UNIT DATA FROM A FILE / GLOBAL VARIABLE


#include "customization.hpp"
#include "skillTree.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Customization::Customization(const std::string& file) {
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

void Customization::handleEvent(const SDL_Event& e) {
if (e.type == SDL_KEYDOWN) {
		// Move to the combat state upon ANY key press
		changeState(new SkillTree());
		// Set the text rendering colour back to normal
		Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
	}
}

void Customization::update(int delta) {

}
//renders Unit data where x,y is the left top corner of the unit data box and unit is the unit to be render
void Customization::renderUnit(int x, int y, UnitData unit){
	int margin = 10;
	//draw enclosing lines for unit data boxes
	Core::Renderer::drawLine(ScreenCoord(x+margin, y+margin), ScreenCoord(x+(Core::windowWidth()/2)-margin,y+margin), Colour(0.0, 0.0, 0.0));
	Core::Renderer::drawLine(ScreenCoord(x+margin, y+margin), ScreenCoord(x+margin,y+(Core::windowHeight()/2)-margin), Colour(0.0, 0.0, 0.0));
	Core::Renderer::drawLine(ScreenCoord(x+(Core::windowWidth()/2)-margin,y+ margin), 
		ScreenCoord(x+(Core::windowWidth()/2)-margin,y+(Core::windowHeight()/2)-margin), Colour(0.0, 0.0, 0.0));
	Core::Renderer::drawLine(ScreenCoord(x+margin,y+(Core::windowHeight()/2)-margin), 
		ScreenCoord(x+(Core::windowWidth()/2)-margin,y+(Core::windowHeight()/2)-margin), Colour(0.0, 0.0, 0.0));

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

	// link to skill tree
	Sprite skillTreeLink("res/assets/UI/SkillTreeLink.png");
	skillTreeLink.setSize(160, 60);
	skillTreeLink.setPos(x+(Core::windowWidth()/3), y+(Core::windowHeight()/5)+100);
	skillTreeLink.render();
    Core::Text_Renderer::render("Skill Tree", ScreenCoord(x+(Core::windowWidth()/3), y+(Core::windowHeight()/5)+100), 1.5f);
}

void Customization::render() {

	if (units.size() > 0) renderUnit(0, 0, units[0]);
	if (units.size() > 1) renderUnit(Core::windowWidth()/2, 0, units[1]);
	if (units.size() > 2) renderUnit(0, (int) Core::windowHeight()/2, units[2]);
	if (units.size() > 3) renderUnit((int) Core::windowWidth()/2, (int) Core::windowHeight()/2, units[3]);

}

void Customization::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}
