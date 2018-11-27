#pragma once

#include "../../engine/core.hpp"
#include "../../math/vec.hpp"
#include "../../engine/animatedSprite.hpp"

#include "attack.hpp"
#include "unit.hpp"

#include <nlohmann/json.hpp>

#define PLAYER_DEFAULT_MOVE_COUNTER		20
#define PLAYER_DEFAULT_ATTACK_COUNTER	20 + 16 * 2

// Enumeration to represent the possible player actions
enum class PlayerAction {
	NONE,
	MOVE,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	ATTACK_4
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

// TODO: update player so that the positions are calculated from the grid
class Player : public Unit {

public:

	Player();
	Player(int x, int y);
	Player(int x, int y, const nlohmann::json& data);

	~Player();
		
	void handleEvent(const SDL_Event& event);
	void update(int delta);

	// The render function and its' corresponding helper functions
	virtual void renderBottom(Combat * combat) override;
	virtual void render() override;
	virtual void renderTop(Combat * combat) override;
	void renderTurnUI();
	void renderValidMoves();
	
	// The click function to handle player logic depending on player state when cursor clicked
	void click(Vec2<int> to);
	
	// Various helper methods
	void setPathLine(Vec2<int> dest);

	// The action that is being expected from the player
	PlayerAction current_action;

protected:
	// Override callback function to customize functionality
	void takeDamageCallback(int damage) override;
	void selectCallback() override;

private:

	// helper variables
	void init();

	// The attacks of the player
	Attack attack1;
	Attack attack2;
	Attack attack3;
	Attack attack4;

	// The outline of the player movement path to the cursor
	std::vector<ScreenCoord> path_line;

	// Movement helper functions
	std::vector<ScreenCoord> getPossibleMoves();
	void updatePossibleMoves();
	std::vector<ScreenCoord> possibleMoves;

	// Player sprites
	AnimatedSprite player_sprite;
	Sprite valid_tile;

	bool moved;
};