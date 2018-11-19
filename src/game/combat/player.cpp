#include "player.hpp"

#include "../combat.hpp"
#include "../util/attackloader.hpp"

// TODO: only allow the player to move once during the turn

Player::Player() :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png"),
	attack1(Attacks::get("PUNCH", this)),
	attack2(Attacks::get("RANGED", this))
{
	player_sprite.setSize(sprite_width, sprite_height);

	player_sprite.addAnimation(1, 1);		// IDLE
	player_sprite.addAnimation(0, 0);		// SELECTED
	player_sprite.addAnimation(2, 16);		// ATK MELEE
	player_sprite.addAnimation(17, 31);		// ATK RANGED	
	player_sprite.addAnimation(32, 34);		// TAKE DAMAGE
	player_sprite.addAnimation(35, 35);		// DEAD
}

Player::Player(int x, int y) :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png"),
	attack1(Attacks::get("PUNCH", this)),
	attack2(Attacks::get("RANGED", this))
{
	player_sprite.setSize(300, sprite_height);
	player_sprite.setSourceSize(96, 96);
	position.x() = x;
	position.y() = y;

	player_sprite.addAnimation(1, 1);		// IDLE
	player_sprite.addAnimation(0, 0);		// SELECTED
	player_sprite.addAnimation(2, 16);		// ATK MELEE
	player_sprite.addAnimation(17, 31);		// ATK RANGED	
	player_sprite.addAnimation(32, 34);		// TAKE DAMAGE
	player_sprite.addAnimation(35, 35);		// DEAD
}

Player::~Player() {}

void Player::renderBottom() {
	shadow.render();
	// Render the corresponding UI elements depending on the players current action
	if (current_action == PlayerAction::MOVE && state == UnitState::IDLE) {
		renderValidMoves();
	}
	if (current_action == PlayerAction::ATTACK_1) {
		attack1.renderValidGrid(tile_width, tile_height);
	}
	if (current_action == PlayerAction::ATTACK_2) {
		attack2.renderValidGrid(tile_width, tile_height);
	}
}

void Player::render()
{
	// Render the actual player
	player_sprite.setPos(screenPosition.x(), screenPosition.y());
	player_sprite.render();
}

void Player::renderTop() {
	// If it's the players turn, render player related UI
	if (state == UnitState::IDLE && selected) {
		renderTurnUI();
	}
	renderHealth();
}

void Player::renderTurnUI() {
	
	// Setup the variables to draw the UI correctly
	ScreenCoord pos = screenPosition;
	pos.x() += tile_width;
	if (pos.y() < 0) pos.y() = 0;
	Colour base = Colour(.7f, .7f, .7f);
	Colour select = Colour(.9f, .9f, .9f);

	// The actual drawing of the UI elements
	Core::Renderer::drawRect(pos + ScreenCoord(UI_X_OFFSET, UI_Y_OFFSET), 150, UI_OPTION_HEIGHT, current_action == PlayerAction::MOVE ? select : base);
	Core::Text_Renderer::render("MOVE", pos, 1.f);
	pos.y() += UI_OPTION_HEIGHT;
	Core::Renderer::drawRect(pos + ScreenCoord(UI_X_OFFSET, UI_Y_OFFSET), 150, UI_OPTION_HEIGHT, current_action == PlayerAction::ATTACK_1 ? select : base);
	Core::Text_Renderer::render(attack1.getName(), pos, 1.f);
	pos.y() += UI_OPTION_HEIGHT;
	Core::Renderer::drawRect(pos + ScreenCoord(UI_X_OFFSET, UI_Y_OFFSET), 150, UI_OPTION_HEIGHT, current_action == PlayerAction::ATTACK_2 ? select : base);
	Core::Text_Renderer::render(attack2.getName(), pos, 1.f);
}

void Player::renderValidMoves() {

	valid_tile.setSize(tile_width, tile_height);
	for (ScreenCoord pos : possibleMoves) {
		valid_tile.setPos((pos.x()) * tile_width, (pos.y()) * tile_height);
		valid_tile.render();
	}

	int i = 0;
	for (ScreenCoord pos : path_line) {
		ScreenCoord start;
		ScreenCoord end;
		if (i == 0) {
			i++;
			continue;
		}
		else {
			start = pos;
			end = path_line[i - 1];
		}

		start.x() *= tile_width;
		end.x() *= tile_width;
		start.y() *= tile_height;
		end.y() *= tile_height;

		start.x() += tile_width / 2;
		end.x() += tile_width / 2;
		start.y() += tile_height / 2;
		end.y() += tile_height / 2;

		Core::Renderer::drawLine(start, end, Colour(1, 0, 0));

		i++;
	}
}

void Player::handleEvent(const SDL_Event & event)
{
	// Only handle events for the entity if it is selected
	if (selected && state == UnitState::IDLE) {
		if (event.type == SDL_KEYDOWN) {
			// Move Key
			if (event.key.keysym.sym == SDLK_KP_1) {
				if (!moved) {
					current_action = PlayerAction::MOVE;
					updatePossibleMoves();
				}
			}
			// Attack 1 key
			if (event.key.keysym.sym == SDLK_KP_2) {
				current_action = PlayerAction::ATTACK_1;
			}
			// Attack 2 key
			if (event.key.keysym.sym == SDLK_KP_3) {
				current_action = PlayerAction::ATTACK_2;
			}
		}
		
	}
}

void Player::update(int delta) {
	// Update the player based on its current state	
	switch (state) {
		case UnitState::IDLE: {
			// Do nothing when idling
		} break;
		case UnitState::MOVE: {
			// Move the player towards its destination
			if (compareCounter(PLAYER_DEFAULT_MOVE_COUNTER)) {
				startCounter();
				incrementMovement();
				// If the player reaches the target destination, stop moving it
				if (position.x() == moveTarget.x() && position.y() == moveTarget.y()) {
					state = UnitState::IDLE;
					current_action = PlayerAction::NONE;
					position = moveTarget;
					calculateScreenPosition();
				}
			}
			else {
				incrementCounter();
				calculateScreenPositionMovement();
			}
		} break;
		case UnitState::ATTACK: {
			if (compareCounter(PLAYER_DEFAULT_ATTACK_COUNTER)) {
				state = UnitState::DONE;
				moved = false;
			} else {
				incrementCounter();
			}
		} break;
		default: {
			// Do nothing by default
		} break;
	}
}

void Player::click(Vec2<int> to)
{
	if (state != UnitState::IDLE) return;
	switch (current_action) {
		case PlayerAction::NONE: {
			// do nothing
		} break;
		case PlayerAction::MOVE: {
			// Move the player using the base unit move function
			if (move(*combat, to)) moved = true;
			current_action = PlayerAction::NONE;
		} break;
			// TODO: determine if an attack is valid, and don't execute the attack if it isn't
		case PlayerAction::ATTACK_1: {
			// do the action here
			attack1.attack(to, *combat);
			current_action = PlayerAction::NONE;
			state = UnitState::ATTACK;
			startCounter();
			// TODO: Play animation based on attack type
			player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_MELEE));
			player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
		} break;
		case PlayerAction::ATTACK_2: {
			// do the action here
			attack2.attack(to, *combat);
			current_action = PlayerAction::NONE;
			state = UnitState::ATTACK;
			startCounter();
			// TODO: Play animation based on attack type
			player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
			player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
		} break;
		default: {
			// do nothing
		} break;
	}
}

void Player::setPathLine(Vec2<int> dest) {
	path_line = getPath(*combat, dest);
}

std::vector<ScreenCoord> Player::getPossibleMoves() {
	std::vector<std::vector<ScreenCoord>> open;
	std::vector<ScreenCoord> seen;

	std::vector<ScreenCoord> root;
	root.push_back(position);

	open.push_back(root);
	while (!(open.empty())) {
		std::vector<ScreenCoord> n = open[0];
		open.erase(open.begin());

		if (n.size() <= static_cast<unsigned int>(getMoveSpeed() + 1)) {
			//continue;

			ScreenCoord end_position = n.back();
			if (std::find(seen.begin(), seen.end(), end_position) != seen.end()) {
				continue;
			}

			seen.push_back(end_position);

			std::vector<ScreenCoord> successors = getValidNeighbours(end_position, *combat);
			for (ScreenCoord succ : successors) {
				std::vector<ScreenCoord> s(n);
				s.push_back(succ);
				open.push_back(s);
			}
		}
	}

	return seen;
}

void Player::updatePossibleMoves()
{
	possibleMoves = getPossibleMoves();
}

void Player::takeDamageCallback(int damage) {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::TAKE_DAMAGE));
	// Decide what the next animation is based on whether the player is still alive or not
	if (health <= 0) player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::DEAD));
	else player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
}

void Player::selectCallback() {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::SELECTED));
}