#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"

class SkillTree : public State {

public:

	SkillTree();
	~SkillTree();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

	void displayUnitData(const UnitData& data);

};

struct node{ 
    int data; 
    int id;
    int children_count;
    struct node *children; 
};
typedef node Node;


