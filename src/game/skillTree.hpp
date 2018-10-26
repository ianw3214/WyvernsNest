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
enum NodeStates { Visited,Reachable,Unvisited}; //reachable nodes can be selected from the user to add them to the visited set
struct node{ 
	enum NodeStates state;
    int data; 
    int id;
    int children_count;
    struct node *children; 
	int x_pos; //x position when rendered 
	int y_pos; //y position when rendered 
};
typedef node Node;


