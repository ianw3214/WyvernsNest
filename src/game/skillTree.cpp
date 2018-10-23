#include "skillTree.hpp"


Node * example_tree;

//###TREE FUNCTIONS####

//creates a new node
Node* newNode(int data, int id, int children_count, Node *children) { 
	// Allocate memory for new node  
	Node* node = (Node*)malloc(sizeof(Node)); 

	// Assign data to this node 
	node->data = data; 
	node->id = id;
	node->children_count=children_count;
	// Initialize left and right children as NULL 
	node->children = children; 
	return(node); 
} 



//returns the height of a tree. The heigh of a 1 node tree is 0.
int treeHeight(Node * node) {
    if (!node) return -1;
	int max = -1;

	//iterate through 
	
	for(size_t i = 0; i < node->children_count; i++)
	{
		if(treeHeight((node->children)+i)>max){
			max = treeHeight((node->children)+i);
		}
	}
	
    return 1 + max;
}

//returns the number of nodes of a tree at targetDepth. Always call it with currentDepth = 0
//First node of tree is 0
int numberOfNodesAt(Node * tree, int targetDepth, int currentDepth){
	//searches the tree recursively and adds +1 every time it finds a node with
		//currentdepth=targetdepth
	int numberOfNodes=0;
	
	if (currentDepth==targetDepth){
		return 1;
	}
	else if (currentDepth>targetDepth){
		return 0;
	}
	
	for(size_t i = 0; i < tree->children_count; i++) 
	{
			numberOfNodes+=numberOfNodesAt((tree->children + i),targetDepth,currentDepth+1);
	}

	return numberOfNodes;
	

}


SkillTree::SkillTree() {

	//example of how to make a tree (example_tree)
	Node *n5 = newNode(1, 5, 0, NULL);   
	Node *n6 = newNode(1, 6, 0, NULL);   
	Node *n7 = newNode(1, 7, 0, NULL);   
	Node *n8 = newNode(1, 8, 0, NULL);   

	Node * children2 = (Node *) malloc(sizeof(Node)*4);
	children2[0]=*n5;
	children2[1]=*n6;
	children2[2]=*n7;
	children2[3]=*n8;
	Node *n3 = newNode(1, 3, 4, children2);   
	Node *n4 = newNode(1, 4, 0, NULL);   
	Node *n1 = newNode(1, 1, 1, n3);   
	Node *n2 = newNode(1, 2, 1, n4);   
	
	Node * children1 = (Node *) malloc(sizeof(Node)*2);
	children1[0]=*n1;
	children1[1]=*n2;
	example_tree = newNode(1, 0 , 2, children1);

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


void renderNode(Node *tree, int * already_rendered, int * nodes_per_depth,int screenWidth, int screenHeight, int currentDepth,int parentX, int parentY){
	int node_offset = screenWidth/(nodes_per_depth[currentDepth]+1);//The horizontal distance between nodes

	int x_position = (already_rendered[currentDepth]+1)*node_offset;
	int y_position = (currentDepth+1)*100;
	already_rendered[currentDepth]++;

	//render node
	Sprite sprite("res/test.png");
	sprite.setSize(100, 30);
	sprite.setPos(x_position, y_position);
	sprite.render();

	//render line
	Core::Renderer::drawLine(ScreenCoord(parentX, parentY), ScreenCoord(x_position, y_position), Colour(1.0, 1.0, 1.0));


	//render children nodes
	for(size_t i = 0; i < tree->children_count; i++)
	{
		renderNode(tree->children+i,already_rendered,nodes_per_depth,screenWidth,screenHeight,currentDepth+1,x_position,y_position);
	}

}



void SkillTree::render() {
	
	int screenWidth = 1280;
	int screenHeight = 720;
	

	int height = treeHeight(example_tree);
	int already_rendered[height+1]; //holds the number of nodes already render at each depth 
	int nodes_per_depth[height+1]; //holds the total number of nodes at each depth

	//inits already_render and ndoes_per_depth
	for(size_t i = 0; i <= height; i++){
		already_rendered[i]=0;
		nodes_per_depth[i]=numberOfNodesAt(example_tree, i, 0);
	}
	renderNode(example_tree,already_rendered, nodes_per_depth,screenWidth,screenHeight,0,screenWidth/2,0);
	
// SAMPLE CODE
	// Core::Renderer::clear();
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(0.0, 0.0, 1.0));
	Core::Renderer::drawLine(ScreenCoord(100, 100), ScreenCoord(100, 200), Colour(0.0, 0.0, 1.0));
	Core::Renderer::drawLine(ScreenCoord(100, 100), ScreenCoord(100, 200), Colour(0.0, 0.0, 1.0));
}


void SkillTree::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}





