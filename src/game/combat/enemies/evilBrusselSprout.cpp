#include "evilBrusselSprout.hpp"

#include "../../combat.hpp"
#include "../../util/attackloader.hpp"

#include <fstream>
#include <iostream>

EvilBrusselSprout::EvilBrusselSprout() :
	Enemy(UnitType::ENEMY, "res/assets/enemies/brussel.png", 64, 64)
{
	int STR = 0;
	int DEX = 0;
	int INT = 0;
	int CON = 0;
	// Load enemy data from file
	const std::string filePath = "res/data/enemies/brussel.json";
	std::ifstream file(filePath);
	if (file.is_open())
	{
		json data;
		file >> data;

		json stats = data["MAGE DUDE"];
		STR = stats["STR"];
		DEX = stats["DEX"];
		INT = stats["INT"];
		CON = stats["CON"];
	}
	else
	{
		std::cerr << "ERROR: load Brussel Sprout. All stats default to 0.\n";
	}

	UnitData udata;
	udata.strength = STR;
	udata.dexterity = DEX;
	udata.intelligence = INT;
	udata.constitution = CON;
	setData(udata);
}


EvilBrusselSprout::~EvilBrusselSprout()
{
}

void EvilBrusselSprout::setTileSizeCallback(int width, int height) {
	// TODO: include this in a metadata file
	float width_to_tile = 1.4f;
	float sprite_ratio = 1.8f;
	// Calculate the sprite size based on the width/height
	float width_ratio = static_cast<float>(128 / BRUSSEL_WIDTH_IN_SOURCE);
	sprite_width = static_cast<int>(width_ratio * width_to_tile * width);
	float height_ratio = static_cast<float>(128 / BRUSSEL_HEIGHT_IN_SOURCE);
	sprite_height = static_cast<int>(height_ratio * width * sprite_ratio);
	sprite.setSize(sprite_width, sprite_height);
	calculateScreenPosition();

	// Also set the units height
	unit_height = static_cast<int>(BRUSSEL_HEIGHT_IN_SOURCE * static_cast<float>(sprite_height) / 128.f);
}

void EvilBrusselSprout::takeDamageCallback(int damage) {
	// if (health >= 0)
	// {
	// 	if (health - damage < 0)
	// 	{
	// 		sprite.playAnimation(4);
	// 		sprite.queueAnimation(5);
	// 	}
	// 	else
	// 	{
	// 		sprite.playAnimation(1, 10);
	// 		sprite.queueAnimation(0);
	// 	}
	// }
}

bool EvilBrusselSprout::handleMovement() {
	return false;
}

void EvilBrusselSprout::handleAttack() {
	// TODO
}