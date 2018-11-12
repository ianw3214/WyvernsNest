#include "skillTree.hpp"

/*#######################
##   LOCAL VARIABLES   ##
#########################*/
Node * example_tree;
int screenWidth = 1280;
int screenHeight = 720;

int height;
int *already_rendered; //holds the number of nodes already render at each depth 
int *nodes_per_depth; //holds the total number of nodes at each depth
int node_height = 30;
int node_width = 100;


/*#######################
##  HELPER FUNCTIONS   ##
#########################*/
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

//returns a clicked node if click x,y is within the node sprite (return NULL if node not found)
Node *clickedNode(Node * tree, int x, int y){
	// printf("x: %d, y : %d\n",x,y);
	Node *clicked_node=NULL;
	//iterate down the tree
	//checks if click happened next to current node
	if(x-tree->x_pos<node_width && x-tree->x_pos>0 && y-tree->y_pos<node_height && y-tree->y_pos>0){
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

//if node is reachable then update it to visited and change children to reachable
void updateNodeState(Node * node){
	if(node->state==Reachable){
		node->state=Visited;
		
		for(size_t i = 0; i < node->children_count; i++)
		{
			node->children[i].state=Reachable;
		}
		
	}else{
		printf("Node is either unreachable or already visited\n");
	}
}

//----------RENDERING FUNCTIONS---------
//render node and line 
void renderNode(Node *node, int parent_x,int parent_y,int node_x,int node_y){
	//set x,y positions for node (data structure)
	node->x_pos=node_x;
	node->y_pos=node_y;

	if(node->state==Unvisited){
		//draw line connecting parent and child
		Core::Renderer::drawLine(ScreenCoord(parent_x+(node_width/2), parent_y+(node_height/2)),
			ScreenCoord(node_x+(node_width/2), node_y+(node_height/2)), Colour(1.0, 1.0, 1.0));
		//draw node
		Sprite sprite("res/test.png");
		sprite.setSize(node_width, node_height);
		sprite.setPos(node_x, node_y);
		sprite.render();
	}else if(node->state == Visited){
		//draw line connecting parent and child
		Core::Renderer::drawLine(ScreenCoord(parent_x+(node_width/2), parent_y+(node_height/2)),
			ScreenCoord(node_x+(node_width/2), node_y+(node_height/2)), Colour(1.0, 1.0, 1.0));		
		//draw node
		Sprite sprite("res/test2.png");
		sprite.setSize(node_width, node_height);
		sprite.setPos(node_x, node_y);
		sprite.render();
	}else if(node->state == Reachable){
		//draw line connecting parent and child
		Core::Renderer::drawLine(ScreenCoord(parent_x+(node_width/2), parent_y+(node_height/2)),
			ScreenCoord(node_x+(node_width/2), node_y+(node_height/2)), Colour(1.0, 1.0, 1.0));		
		//draw node
		Sprite sprite("res/test3.png");
		sprite.setSize(node_width, node_height);
		sprite.setPos(node_x, node_y);
		sprite.render();
	}
}

//renders a tree. initial parameters: current_depth = 0. parent_x, parent_y = the origin of the tree
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


/*########################
##   CLASS FUNCTIONS    ##
##########################*/
SkillTree::SkillTree() {
	//----------------------------------------------------//
	//----example of how to make a tree (example_tree)----//
	//----------------------------------------------------//
	//depth4
	Node *n11 = newNode(1, 11, 0, NULL,Unvisited);  
	//depth3
	Node *n5 = newNode(1, 5, 0, NULL,Unvisited);   
	Node *n6 = newNode(1, 6, 0, NULL,Unvisited);   
	Node *n7 = newNode(1, 7, 1, n11,Unvisited);   
	Node *n8 = newNode(1, 8, 0, NULL,Unvisited);   
	Node * children3 = (Node *) malloc(sizeof(Node)*4);
	children3[0]=*n5;
	children3[1]=*n6;
	children3[2]=*n7;
	children3[3]=*n8;
	//depth2
	Node *n9 = newNode(1, 9, 4, children3,Unvisited);   
	Node *n10 = newNode(1, 10, 0, NULL,Unvisited);   
	Node * children2 = (Node *) malloc(sizeof(Node)*2);
	children2[0]=*n9;
	children2[1]=*n10;
	Node *n3 = newNode(1, 3, 0, NULL,Unvisited);
	//depth1
	Node *n1 = newNode(1, 1, 1, n3,Unvisited);   
	Node *n2 = newNode(1, 2, 2, children2,Unvisited);   
	Node * children1 = (Node *) malloc(sizeof(Node)*2);
	children1[0]=*n1;
	children1[1]=*n2;
	//depth 0
	example_tree = newNode(1, 0 , 2, children1,Reachable);

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

		//call relevant functions to update selected node
		Node * clicked_node=clickedNode(example_tree, mouseX,mouseY);
		if(clicked_node!=NULL){
			updateNodeState(clicked_node);
		}
	}
}

void SkillTree::update(int delta) {

}

void SkillTree::render() {
	//populates already_rendered and nodes_per_depth with initial values
	for(size_t i = 0; i <= height; i++){
		already_rendered[i]=0;
		nodes_per_depth[i]=numberOfNodesAt(example_tree, i, 0);
	}
	//renders tree
	renderTree(example_tree,0,screenWidth/2,0);
	
}
