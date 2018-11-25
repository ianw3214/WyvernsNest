#pragma once

#include "../engine/core.hpp"

#include "unitData.hpp"

#define DEFAULT_SKILL_TREE_PATH		"res/data/tree.json"
#define DEFAULT_PLAYER_FILE			"res/data/players.json"

class Node {
public:
	std::string data;			// data that shows when hovering on node
	int id;
	std::string spritePath;		// path to sprite of the node e.g. 'res/fistbump.png'
	std::vector<int> children;	// children ids

	int x_offset;				// x offset from the centre of the tree from -5 to 5
	int level;					// height level in the tree. Starts at 0.

	int x_position;				// x position of node when rendered. -1 by default
	int y_position;				// y position of node when rendeered. -1 by defaults

	// Constructor
	Node(std::string data, int id, std::string spritePath,
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
	int height;
	// Keep track of all selected nodes
	std::vector<int> selected;
	std::vector<int> reachable;

	void initSprites();
	// Sprites used to render the customization state
	Sprite base;
	Sprite sprite_empty;
	Sprite sprite_reachable;

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