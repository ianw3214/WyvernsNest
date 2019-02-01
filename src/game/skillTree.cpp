#include "skillTree.hpp"

#include <algorithm>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "customization.hpp"

//creates a new node
Node::Node(std::string n_data, int n_id, int n_sprite_index,
	std::vector<int> n_children, int n_x_offset, int n_level){
	data=n_data;
	id=n_id;
	sprite_index = n_sprite_index;
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
	int top_margin = 40;
	// DEFAULT ALL NODES TO 50/50 FOR NOW
	// TODO: Use a better system to render node sprites
	int node_width = 50;
	int node_height = 50;
	int x_pos = (Core::windowWidth()/2) + node->x_offset*200 - (node_width/2);
	int y_pos = top_margin + (node->level*100);
	// Set x,y positions for node (data structure)

	// Draw node base
	if (std::find(selected.begin(), selected.end(), node->id) != selected.end()) {
		// Check if selected
		sprite_selected.setSize(node_width, node_height);
		sprite_selected.setPos(x_pos, y_pos);
		sprite_selected.render();
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
	// Draw node sprite
	icons.setSourcePos(NODE_X(node->sprite_index), NODE_Y(node->sprite_index));
	icons.setSize(node_width, node_height);
	icons.setPos(x_pos, y_pos);
	icons.render();

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
		std::ifstream f(DEFAULT_PLAYER_FILE_SKILLTREE);
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
		index++;
	}
	{	// Write the results to a file
		std::ofstream f(DEFAULT_PLAYER_FILE_SKILLTREE);
		f << data.dump(4);
	}
	changeState(new Customization());
}

SkillTree::SkillTree(int playerId, const std::string & skillTreePath) :
	playerId(playerId),
	selected(selected),
	base("res/assets/UI/SkillTreeBase.png"),
	sprite_selected("res/assets/UI/NodeBase.png"),
	sprite_empty("res/assets/UI/NodeEmpty.png"),
	sprite_reachable("res/assets/UI/NodeReachable.png"),
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png"),
	icons(NODE_ICONS_FILE)
{
	{	// Read skill tree data from a file
		std::ifstream f(skillTreePath);
		json data;
		f >> data;
		height = data["height"];
		for (const json& node : data["nodes"]) {
			int id = node["id"];
			std::string data = node["data"];
			int sprite_index = node["sprite_index"];
			int x_offset = node["x_offset"];
			int level = node["level"];
			// Construct the node object and add its children
			Node obj = Node(data, id, sprite_index, std::vector<int>(), x_offset, level);
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
				playerLevel = player["level"];
				break;
			}
			index++;
		}
	}

	// Initialize skilltree state
	initSprites();
	updateReachableNodes();
	backButton = ButtonData(ScreenCoord(Core::windowWidth() / 2 - 64, Core::windowHeight() - 90), 128, 64);
	backButton.setSprites("res/assets/UI/BackButton.png", "res/assets/UI/BackButtonHover.png", "res/assets/UI/BackButton.png");
	icons.setSourceSize(NODE_ICONS_SOURCE_SIZE, NODE_ICONS_SOURCE_SIZE);
}

SkillTree::~SkillTree() {

}

void SkillTree::handleEvent(const SDL_Event& e) {

	//check if node clicked 
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// If we can choose points AND the mouse clicked on a node, select it
		if (playerLevel - selected.size() > 0) {
			int collidingNode = nodeColliding(mouseX, mouseY);
			if (collidingNode != -1) {
				if (std::find(reachable.begin(), reachable.end(), collidingNode) != reachable.end()) {
					selectNode(collidingNode);
				}
			}
		}

		if (backButton.colliding(ScreenCoord(mouseX, mouseY))) {
			returnToCustomization();
		}
	}

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
			returnToCustomization();
		}
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			exit(0);
		}
	}
	// Update the mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
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
	
	// Render buttons
	backButton.render();

	// Render UI
	Core::Text_Renderer::setColour(Colour(.2f, .2f, .2f));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	std::string text = std::string("Available skill points: ") + std::to_string(playerLevel - selected.size());
	Core::Text_Renderer::render(text, ScreenCoord(10, Core::windowHeight() - 20));

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	} else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void SkillTree::initSprites() {
	base.setSize(Core::windowWidth(), Core::windowHeight());
}
