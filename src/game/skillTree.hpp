#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"
#include "util/button.hpp"

#define DEFAULT_SKILL_TREE_PATH			"res/data/tree.json"
#define DEFAULT_PLAYER_FILE_SKILLTREE	"res/data/save.json"

#define NODE_ICONS_FILE					"res/assets/UI/nodeIcons.png"
#define NODE_ICONS_FILE_WIDTH			10
#define NODE_ICONS_FILE_HEIGHT			10
#define NODE_ICONS_SOURCE_SIZE			32
#define NODE_X(i)						((i % NODE_ICONS_FILE_WIDTH) * NODE_ICONS_SOURCE_SIZE)
#define NODE_Y(i)						((NODE_ICONS_FILE_HEIGHT - (i / NODE_ICONS_FILE_WIDTH) - 1) * NODE_ICONS_SOURCE_SIZE)

class Node {
public:
	std::string data;			// data that shows when hovering on node
	int id;
	int sprite_index;			// Index of the node icon in the node icons file
	std::vector<int> children;	// children ids

	int x_offset;				// x offset from the centre of the tree from -5 to 5
	int level;					// height level in the tree. Starts at 0.

	int x_position;				// x position of node when rendered. -1 by default
	int y_position;				// y position of node when rendeered. -1 by defaults

	// Constructor
	Node(std::string data, int id, int sprite_index,
		std::vector<int> children, int x_offset, int level);
};

class SkillTree : public State { 

public:

	// NOTE: The playerId is really just the index into the json file
	SkillTree(int playerId, const std::string& skillTreePath = DEFAULT_SKILL_TREE_PATH);
	~SkillTree();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

	int playerId;
	int playerLevel;
	int height;
	// Keep track of all selected nodes
	std::vector<int> selected;
	std::vector<int> reachable;

	// State variables
	int mouseX, mouseY;
	bool mouseDown;

	void initSprites();
	// Sprites used to render the customization state
	Sprite base;
	Sprite sprite_selected;
	Sprite sprite_empty;
	Sprite sprite_reachable;
	Sprite icons;
	Sprite cursor;
	Sprite cursorPress;

	// Buttons
	ButtonData backButton;

	// List of all nodes
	std::vector<Node> nodes;

	// Node helper functions
	Node * getNodeById(int id);
	int nodeColliding(int x, int y);
	void renderNode(Node * node);
	void selectNode(int id);
	void updateReachableNodes();

	// Other helper functions
	void returnToCustomization();
};