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
//reachable nodes can be selected by the user in order to add them to the visited set

int screenWidth = 1280;
int screenHeight = 720; 

};
enum NodeStates { Visited,Reachable,Unvisited}; 
struct node{ 
	enum NodeStates state; //visited, reachable or unvisited
    std::string data; //data that shows when hovering on node
    int id;
	std::string spritePath; //path to sprite of the node e.g. 'res/fistbump.png'
	std::vector<int> children; //children ids

	int x_offset; //x offset from the centre of the tree from -5 to 5
	int level; //height level in the tree. Starts at 0.

	int x_position; //x position of node when rendered. -1 by default
	int y_position; //y position of node when rendeered. -1 by defaults
};
typedef node Node;
