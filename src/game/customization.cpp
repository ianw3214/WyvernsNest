#include "customization.hpp"


Customization::Customization() {

	//EXAMPLE OF HOW TO SET UP A TREE 
	//the information to actually set it up will be read from a file in the future
	Skill_tree *example_tree = (Skill_tree *) malloc(sizeof(Skill_tree));

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

Customization::~Customization() {

}

void Customization::handleEvent(const SDL_Event& e) {

}

void Customization::update(int delta) {

}

void Customization::render() {
// SAMPLE CODE
	// Core::Renderer::clear();
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));
	Sprite sprite("res/test.png");
	sprite.setPos(100, 100);
	sprite.setSize(100, 100);
	sprite.render();	
}

void Customization::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}





