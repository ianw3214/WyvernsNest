#include "skillTree.hpp"

Skill_tree *example_tree;

SkillTree::SkillTree() {

	//EXAMPLE OF HOW TO SET UP A TREE 
	//the information to actually set it up will be read from a file in the future
	example_tree = (Skill_tree *) malloc(sizeof(Skill_tree));

	//nodes
	char * content_example = (char *) malloc(sizeof(char)*5);
	strcpy(content_example,"test\0");

	Tree_node * nodes = (Tree_node *) malloc(sizeof(Tree_node)*3);
	nodes[0].id=1;
	nodes[0].content=content_example;
	nodes[1].id=2;
	nodes[1].content=content_example;
	nodes[2].id=3;
	nodes[2].content=content_example;

	Tree_edge * edges = (Tree_edge *) malloc(sizeof(Tree_node)*2);
	edges[0].from=1;
	edges[0].to=2;
	edges[1].from=1;
	edges[1].to=3;

	example_tree->nodes=nodes;
	example_tree->nodes=nodes;
	example_tree->number_of_edges=2;
	example_tree->number_of_nodes=3;

}

SkillTree::~SkillTree() {

}

void SkillTree::handleEvent(const SDL_Event& e) {

}

void SkillTree::update(int delta) {

}
int fakeFunction(){
	return 100;
}

void SkillTree::render() {
	int number_of_nodes = example_tree->number_of_nodes;
	
	Sprite sprite("res/test.png");
	sprite.setSize(100, 100);
		
	for(size_t i = 0; i < number_of_nodes; i++)
	{
		sprite.setPos(100*i, 100*i);
		sprite.render();
	}
	
// SAMPLE CODE
	// Core::Renderer::clear();
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(0.0, 0.0, 1.0));
	Core::Renderer::drawLine(ScreenCoord(100, 100), ScreenCoord(100, 200), Colour(0.0, 0.0, 1.0));
	Core::Renderer::drawLine(ScreenCoord(100, 100), ScreenCoord(100, 200), Colour(0.0, 0.0, 1.0));
}


void SkillTree::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}





