#include "skillTree.hpp"

#include <algorithm>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "customization.hpp"

//creates a new node
Node::Node(std::string n_data, int n_id, std::string n_spritePath,
	std::vector<int> n_children, int n_x_offset, int n_level) {
	data=n_data;
	id=n_id;
	spritePath=n_spritePath;
	children=n_children;
	x_offset=n_x_offset;
	level=n_level;
	x_position=-1;
	y_position=-1;
}

Node * SkillTree::getNodeById(int id){
	for (Node& n : nodes){
		if(n.id==id){
			return & n;
		}
	}
	return nullptr;
}
// //returns the id of the clicked node. Return -1 if no node was clicked.
int SkillTree::nodeColliding(int x, int y) {
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
// Render node along with children edges
void SkillTree::renderNode(Node * node) {
	int top_margin = 60;
	// DEFAULT ALL NODES TO 50/50 FOR NOW
	// TODO: Use a better system to render node sprites
	int node_width = 50;
	int node_height = 50;
	int x_pos = (Core::windowWidth()/2) + node->x_offset*200 - (node_width/2);
	int y_pos = top_margin + (node->level*100);
	// Set x,y positions for node (data structure)

	// Draw node
	if (std::find(selected.begin(), selected.end(), node->id) != selected.end()) {
		Sprite sprite(node->spritePath.c_str());
		sprite.setSize(node_width, node_height);
		sprite.setPos(x_pos, y_pos);
		sprite.render();
	} else {
		// Check if reachable
		if (std::find(reachable.begin(), reachable.end(), node->id) != reachable.end()) {
			sprite_reachable.setSize(node_width, node_height);
			sprite_reachable.setPos(x_pos, y_pos);
			sprite_reachable.render();
		} else {
			sprite_empty.setSize(node_width, node_height);
			sprite_empty.setPos(x_pos, y_pos);
			sprite_empty.render();
		}
	}

	// Update location tracking (upper left corner)
	node->x_position=x_pos;
	node->y_position=y_pos;

	// Draw edges connecting nodes to children
	int child_x=-1;
	int child_y=-1;
	for (int& child_id : node->children){	
		int child_x = (Core::windowWidth()/2) + getNodeById(child_id)->x_offset*200;
		int child_y = top_margin + getNodeById(child_id)->level*100;
		Core::Renderer::drawLine(ScreenCoord(x_pos+(node_width/2),y_pos+node_height),ScreenCoord(child_x,child_y), Colour(1.0, 1.0, 1.0));	
	}
	
}

void SkillTree::selectNode(int id) {
	if (std::find(selected.begin(), selected.end(), id) != selected.end()) return;
	selected.push_back(id);
	updateReachableNodes();
}

void SkillTree::updateReachableNodes() {
	reachable.clear();
	for (int id : selected) {
		for (int child : getNodeById(id)->children) {
			if (getNodeById(child)->level < static_cast<int>(selected.size())) continue;
			if (std::find(reachable.begin(), reachable.end(), child) == reachable.end()) reachable.push_back(child);
		}
	}
}

void SkillTree::returnToCustomization() {
	// Save the changes made to the tree and return to the customization state
	json data;
	{	// Read player skill tree data from a file
		std::ifstream f(DEFAULT_PLAYER_FILE);
		f >> data;
	}
	// Find the player and change its selected nodes
	int index = 0;
	for (json& player : data["players"]) {
		if (index == playerId) {
			// Update the players children if it has been found
			player["selected"] = selected;
			break;
		}
	}
	{	// Write the results to a file
		std::ofstream f(DEFAULT_PLAYER_FILE);
		f << data.dump(4);
	}
	changeState(new Customization());
}

SkillTree::SkillTree(int playerId, const std::string & skillTreePath) :
	playerId(playerId),
	selected(selected),
	base("res/assets/UI/SkillTreeBase.png"),
	sprite_empty("res/assets/UI/NodeEmpty.png"),
	sprite_reachable("res/assets/UI/NodeReachable.png")
{
	{	// Read skill tree data from a file
		std::ifstream f(skillTreePath);
		json data;
		f >> data;
		height = data["height"];
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
	}

	{	// Read player skill tree data from a file
		std::ifstream f(DEFAULT_PLAYER_FILE);
		json data;
		f >> data;
		int index = 0;
		for (const json& player : data["players"]) {
			// Found the player if ID same
			if (index == playerId) {
				for (int child : player["selected"]) {
					selected.push_back(child);
				}
				break;
			}
			index++;
		}
	}

	// Initialize skilltree state
	initSprites();
	updateReachableNodes();
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
		int collidingNode = nodeColliding(mouseX,mouseY);
		if (collidingNode != -1) {
			if (std::find(reachable.begin(), reachable.end(), collidingNode) != reachable.end()) {
				selectNode(collidingNode);
			}
		}
	}

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_a) {
			returnToCustomization();
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
