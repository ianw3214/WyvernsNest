#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "attack.hpp"
#include "unit.hpp"
#include "node.hpp"
#include "../../engine/animatedSprite.hpp"

#define PLAYER_DEFAULT_MOVE_COUNTER		20
#define PLAYER_DEFAULT_ATTACK_COUNTER	20 + 16 * 2

#define UI_X_OFFSET					   -10
#define UI_Y_OFFSET					    5
#define UI_OPTION_HEIGHT				30

// Enumeration to represent the possible player actions
enum class PlayerAction {
	NONE,
	MOVE,
	ATTACK_1,
	ATTACK_2
};

// Enumeration to represent player animation states
enum class PlayerAnim {
	IDLE = 0,
	SELECTED = 1,
	ATTACK_MELEE = 2,
	ATTACK_RANGED = 3,
	TAKE_DAMAGE = 4,
	DEAD = 5
};

class Combat;

// TODO: update player so that the positions are calculated from the grid
class Player : public Unit {

public:

	Player();
	Player(int x, int y);
	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);

	// The render function and its' corresponding helper functions
	void render();
	void renderTurnUI();
	void renderValidMoves();
	
	// The click function to handle player logic depending on player state when cursor clicked
	void click(Vec2<int> to, Combat& combat);
	
	// Various helper methods
	void setPathLine(Combat& combat, Vec2<int> dest);

	// The action that is being expected from the player
	PlayerAction current_action;

protected:
	// Override callback function to customize functionality
	void takeDamageCallback(int damage) override;
	void selectCallback() override;

private:

	// The outline of the player movement path to the cursor
	std::vector<ScreenCoord> path_line;

	std::vector<ScreenCoord> getPossibleMoves(Combat & combat);
	void updatePossibleMoves(Combat & combat);
	std::vector<ScreenCoord> possibleMoves;

	// Player sprites
	AnimatedSprite player_sprite;
	Sprite valid_tile;

	bool moved;
};