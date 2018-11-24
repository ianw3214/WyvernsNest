#include "skillTree.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/*#######################
##  HELPER FUNCTIONS   ##
#########################*/

//creates a new node
Node::Node(std::string n_data, int n_id, std::string n_spritePath,
	std::vector<int> n_children, int n_x_offset, int n_level){
	data=n_data;
	id=n_id;
	spritePath=n_spritePath;
	children=n_children;
	x_offset=n_x_offset;
	level=n_level;
	x_position=-1;
	y_position=-1;
}

Node * getNodeById(int id){
	
	for (Node& n : nodes){
		if(n.id==id){
			return & n;
		}
	}
	fprintf(stderr,("node with id " + std::to_string(id) + " wasn't found").c_str());
	return NULL;
	
}
// //returns the id of the clicked node. Return -1 if no node was clicked.
int nodeColliding(int x, int y){
	int node_width = 50;
	int node_height = 50;
	
	//check node which position is close to x,y
	for (Node& n : nodes){
		if(x-n.x_position<=node_width && x-n.x_position>=0 
			&& y-n.y_position<=node_height &&y-n.y_position>=0){
			return n.id;
		}
	}

	return -1;
	
}

//----------RENDERING FUNCTIONS---------
//render node along with children edges
void renderNode(Node * node){
	int top_margin = 60;
	// DEFAULT ALL NODES TO 50/50 FOR NOW
	// TODO: Use a better system to render node sprites
	int node_width = 50;
	int node_height = 50;
	int x_pos = (Core::windowWidth()/2) + node->x_offset*200 - (node_width/2);
	int y_pos = top_margin + (node->level*100);
	//set x,y positions for node (data structure)

	//draw node
	Sprite sprite(node->spritePath.c_str());
	sprite.setSize(node_width, node_height);
	sprite.setPos(x_pos, y_pos);
	sprite.render();

	//update location tracking (upper left corner)
	node->x_position=x_pos;
	node->y_position=y_pos;

	//draw edges connecting nodes to children
	int child_x=-1;
	int child_y=-1;
	for (int& child_id : node->children){	
		int child_x = (Core::windowWidth()/2) + getNodeById(child_id)->x_offset*200;
		int child_y = top_margin + getNodeById(child_id)->level*100;
		Core::Renderer::drawLine(ScreenCoord(x_pos+(node_width/2),y_pos+node_height),ScreenCoord(child_x,child_y), Colour(1.0, 1.0, 1.0));	

	}
	
}


/*########################
##   CLASS FUNCTIONS    ##
##########################*/

SkillTree::SkillTree(const std::string & path) :
	base("res/assets/UI/SkillTreeBase.png")
{
	std::ifstream f(path);
	json data;
	f >> data;

	for (const json& node : data["nodes"]) {

		int id = node["id"];
		std::string data = node["data"];
		std::string sprite = node["sprite"];
		int x_offset = node["x_offset"];
		int level = node["level"];

		// Construct the node object and add its children
		Node obj = Node(data, id, sprite, std::vector<int>(), x_offset, level);
		for (int child : node["children"]) {
			obj.children.push_back(child);
		}
		nodes.push_back(obj);
	}

	// Initialize skilltree state
	initSprites();
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
		int node_id = nodeColliding(mouseX,mouseY);
		printf("%d\n",node_id);
		if(node_id!=-1){
			// printf("%d\n",node_id);
		}
	}
}

void SkillTree::update(int delta) {
}

void SkillTree::render() {
	// Render background sprite
	base.render();

	// Render the nodes
	for(Node &n:nodes){
		renderNode(&n);
	}

	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Render a description of the node if is colliding with the mouse
	int collidingNode = nodeColliding(mouseX, mouseY);
	if (collidingNode != -1) {
		Node * node = getNodeById(collidingNode);
		// TODO: Render more descriptions and automatically calculate margins based on screen position
		Core::Text_Renderer::setColour(Colour(.2f, .2f, .2f));
		Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
		Core::Text_Renderer::render(node->data, Vec2<int>(node->x_position + 50, node->y_position));
	}
	
}

void SkillTree::initSprites() {
	base.setSize(Core::windowWidth(), Core::windowHeight());
}
