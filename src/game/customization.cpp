//STILL NEEDS TO INCLUDE LINK TO SKILL TREE 
//ALSO NEEDS TO READ UNIT DATA FROM A FILE / GLOBAL VARIABLE


#include "customization.hpp"

Customization::Customization() {

	//generate data for units
	generateDefaultUnitData();
}

Customization::~Customization() {

}

//only generates dummy data right now
void Customization::generateDefaultUnitData() {

	// Default attributes for each unit1
	unit1.name = "BOB";unit1.strength = 10;unit1.dexterity = 3;unit1.intelligence = 4;unit1.constitution = 100;
	unit2.name = "SAM";unit2.strength = 1;unit2.dexterity = 3;unit2.intelligence = 4;unit2.constitution = 100;
	unit3.name = "TIM";unit3.strength = 10;unit3.dexterity = 3;unit3.intelligence = 4;unit3.constitution = 100;
	unit4.name = "MR. CHERRY";unit4.strength = 40;unit4.dexterity = 3;unit4.intelligence = 4;unit4.constitution = 100;
}

void Customization::handleEvent(const SDL_Event& e) {

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
	Sprite unitSprite("res/FemaleattackTEST.gif");
	unitSprite.setSize(windowWidth/4, windowHeight/2.5);
	unitSprite.setPos(x+margin+(margin/2), y+margin+(margin/2));
	unitSprite.render();

	//unit name
	Core::Text_Renderer::setColour(Colour(0,0,0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
    Core::Text_Renderer::render(unit.name, ScreenCoord(x+(windowWidth/4), y), 3.f);

	//attributes
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
    Core::Text_Renderer::render("Strength: " + std::to_string(unit.strength), ScreenCoord(x+(windowWidth/5), y+(windowHeight/5)), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.dexterity), ScreenCoord(x+(windowWidth/5), y+(windowHeight/5)+30), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.intelligence), ScreenCoord(x+(windowWidth/5), y+(windowHeight/5)+60), 1.f);
    Core::Text_Renderer::render("Dexterity: " + std::to_string(unit.constitution), ScreenCoord(x+(windowWidth/5), y+(windowHeight/5)+90), 1.f);

	//link to skill tree
	Sprite skillTreeLink("res/test8.png");
	skillTreeLink.setSize(160, 60);
	skillTreeLink.setPos(x+(windowWidth/2.9), y+(windowHeight/5)+100);
	skillTreeLink.render();
    Core::Text_Renderer::render("Skill Tree", ScreenCoord(x+(windowWidth/2.9), y+(windowHeight/5)+100), 1.5f);
}

void Customization::render() {

	renderUnit(0,0,unit1);
	renderUnit(windowWidth/2,0,unit2);
	renderUnit(0,(int) windowHeight/2,unit3);
	renderUnit((int) windowWidth/2,(int) windowHeight/2,unit4);
	for(size_t i = 0; i < 4; i++)
	{	
		//Render the 4 units on screen
	}
	

}

void Customization::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}
