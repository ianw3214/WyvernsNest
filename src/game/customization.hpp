#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"

class Customization : public State {

public:

	Customization();
	~Customization();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

	int windowWidth = 1280;
	int windowHeight = 720;
	UnitData unit1, unit2, unit3, unit4;

	void displayUnitData(const UnitData& data);
	void renderUnit(int x, int y, UnitData unit);
	void generateDefaultUnitData();

};