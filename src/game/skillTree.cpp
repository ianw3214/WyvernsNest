#include "skillTree.hpp"
#include <cmath>

using namespace std;

//###TREE VARIABLES###
Node * example_tree;
int screenWidth = 1280;
int screenHeight = 720;
int height;
int *already_rendered; //holds the number of nodes already render at each depth 
int *nodes_per_depth; //holds the total number of nodes at each depth

//###TREE FUNCTIONS####
//creates a new node
Node* newNode(int data, int id, int children_count, Node *children,enum NodeStates state) { 
	// Allocate memory for new node  
	Node* node = (Node*)malloc(sizeof(Node)); 

	// Assign data to this node 
	node->data = data; 
	node->id = id;
	node->children_count=children_count;
	// Initialize left and right children as NULL 
	node->children = children; 
	node->state=state;
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

//returns the number of nodes of a tree at targetDepth. Always call it with current_depth = 0
//First node of tree is 0
int numberOfNodesAt(Node * tree, int targetDepth, int current_depth){
	//searches the tree recursively and adds +1 every time it finds a node with
		//current_depth=targetdepth
	int numberOfNodes=0;
	
	if (current_depth==targetDepth){
		return 1;
	}
	else if (current_depth>targetDepth){
		return 0;
	}
	
	for(size_t i = 0; i < tree->children_count; i++) 
	{
			numberOfNodes+=numberOfNodesAt((tree->children + i),targetDepth,current_depth+1);
	}

	return numberOfNodes;
	

}

//returns a clicked node near an x and y position (return NULL if node not found)
Node *clickedNode(Node * tree, int x, int y){
	// printf("x: %d, y : %d\n",x,y);
	Node *clicked_node=NULL;
	//iterate down the tree
	//checks if click happened next to current node
	if(abs(x-tree->x_pos)<50 && abs(y-tree->y_pos)<15){
		return tree;
	}else if(tree->children!=NULL){
		
		for(size_t i = 0; i < tree->children_count; i++)
		{	
			if(clickedNode(&tree->children[i],x,y)!=NULL){
				clicked_node=clickedNode(&tree->children[i],x,y);
			}
		}
	}
	return clicked_node;
}

//Constructor
SkillTree::SkillTree() {
	//example of how to make a tree (example_tree)
	//note: it needs to be made bottom up
	Node *n5 = newNode(1, 5, 0, NULL,Unvisited);   
	Node *n6 = newNode(1, 6, 0, NULL,Unvisited);   
	Node *n7 = newNode(1, 7, 0, NULL,Visited);   
	Node *n8 = newNode(1, 8, 0, NULL,Unvisited);   

	Node * children2 = (Node *) malloc(sizeof(Node)*4);
	children2[0]=*n5;
	children2[1]=*n6;
	children2[2]=*n7;
	children2[3]=*n8;

	Node *n3 = newNode(1, 3, 4, children2,Visited);   
	Node *n4 = newNode(1, 4, 0, NULL,Unvisited);   
	Node *n1 = newNode(1, 1, 1, n3,Visited);   
	Node *n2 = newNode(1, 2, 1, n4,Unvisited);   
	
	Node * children1 = (Node *) malloc(sizeof(Node)*2);
	children1[0]=*n1;
	children1[1]=*n2;
	example_tree = newNode(1, 0 , 2, children1,Visited);


	//init local variables
	height = treeHeight(example_tree);
	already_rendered = (int *)malloc(sizeof(int)*(height+1));
	nodes_per_depth = (int *)malloc(sizeof(int)*(height+1));
	
}

SkillTree::~SkillTree() {

}

void SkillTree::handleEvent(const SDL_Event& e) {

	//check if node clicked 
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int mouseX;
		int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Node * clicked_node=clickedNode(example_tree, mouseX,mouseY);
	if(clicked_node!=NULL){
	printf("id: %d\n",clicked_node->id);
	}
	}


}

void SkillTree::update(int delta) {

}

//render node and line 
void renderNode(Node *node, int parent_x,int parent_y,int node_x,int node_y){
	//set x,y positions for node
	node->x_pos=node_x;
	node->y_pos=node_y;

	if(node->state==Unvisited){
		Core::Renderer::drawLine(ScreenCoord(parent_x, parent_y), ScreenCoord(node_x, node_y), Colour(1.0, 1.0, 1.0));
		Sprite sprite("res/test.png");
		sprite.setSize(100, 30);
		sprite.setPos(node_x, node_y);
		sprite.render();
	}else if(node->state == Visited){
		Core::Renderer::drawLine(ScreenCoord(parent_x, parent_y), ScreenCoord(node_x, node_y), Colour(0.0, 0.0, 1.0));
		Sprite sprite("res/visited.png");
		sprite.setSize(100, 30);
		sprite.setPos(node_x, node_y);
		sprite.render();
	}


}

//renders a node and it's children (along with the lines connecting them)
void renderTree(Node *tree, int current_depth,int parent_x, int parent_y){
	int node_offset = screenWidth/(nodes_per_depth[current_depth]+1);//The horizontal distance between nodes at the given depth
	int x_position = (already_rendered[current_depth]+1)*node_offset;
	int y_position = (current_depth+1)*100;

	//render node and line 
	renderNode(tree,parent_x,parent_y,x_position,y_position);
	already_rendered[current_depth]++; //update the number of nodes rendered at that depth
	//render children nodes
	//recursively calls renderNode in the children of the current node
	for(size_t i = 0; i < tree->children_count; i++){
		renderTree(tree->children+i,current_depth+1,x_position,y_position);
	}

}


void SkillTree::render() {
	//populates already_rendered and nodes_per_depth with initial values
	for(size_t i = 0; i <= height; i++){
		already_rendered[i]=0;
		nodes_per_depth[i]=numberOfNodesAt(example_tree, i, 0);
	}
	//render tree
	renderTree(example_tree,0,screenWidth/2,0);
	
}


void SkillTree::displayUnitData(const UnitData & data) {
	// Render the unit data to the window...
}





