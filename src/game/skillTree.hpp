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




/* OLD DEFINITION OF TREE
//Data structure to hold a tree
typedef struct {
	int id;
	char *content;
} Tree_node;

typedef struct {
  int from; //id of the first node
  int to; //id of the second node
} Tree_edge; //edge between two nodes in a tree

typedef struct {
  Tree_node *nodes;
  Tree_edge *edges;
  int number_of_nodes;
  int number_of_edges;
} Skill_tree;

*/