#include "player.hpp"
#include "../combat.hpp"

// TODO: only allow the player to move once during the turn

Player::Player() :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/FemaleattackTEST.png", 96, 96),
	valid_tile("res/assets/valid.png")
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
	player_sprite("res/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/valid.png")
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

void Player::render()
{
	// Render the shadow first
	shadow.render();

	// Render the corresponding UI elements depending on the players current action
	if (current_action == PlayerAction::MOVE && state == UnitState::IDLE) {
		renderValidMoves();
	}
	if (current_action == PlayerAction::ATTACK_1) {
		attack1.renderValidGrid();
	}
	if (current_action == PlayerAction::ATTACK_2) {
		attack2.renderValidGrid();
	}
	// Render the actual player
	player_sprite.setPos(screenPosition.x(), screenPosition.y());
	player_sprite.render();
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
					updatePossibleMoves(*combat);
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

void Player::click(Vec2<int> to, Combat& combat)
{
	if (state != UnitState::IDLE) return;
	switch (current_action) {
		case PlayerAction::NONE: {
			// do nothing
		} break;
		case PlayerAction::MOVE: {
			// Only move the player to empty positions
			if (combat.isPosEmpty(to)) {
				// Also check if the movement is valid first
				int steps = std::abs(to.x() - position.x()) + std::abs(to.y() - position.y());
				if (steps <= getMoveSpeed()) {
					moveTarget = to;
					path = getPath(combat, moveTarget);
					if (path.size() > 0) {
						moveNext = ScreenCoord(0, 0);
						incrementMovement();
						state = UnitState::MOVE;
						startCounter();
						moved = true;
					}
					else {
						//incorrect pos
						current_action = PlayerAction::NONE;
					}
				}
			}
		} break;
			// TODO: determine if an attack is valid, and don't execute the attack if it isn't
		case PlayerAction::ATTACK_1: {
			// do the action here
			attack1.attack(to, combat);
			current_action = PlayerAction::NONE;
			state = UnitState::ATTACK;
			startCounter();
			// TODO: Play animation based on attack type
			player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_MELEE));
			player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
		} break;
		case PlayerAction::ATTACK_2: {
			// do the action here
			attack2.attack(to, combat);
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

void Player::calculateScreenPositionMovement() {
		screenPosition.x() += moveNext.x() * tile_width / 20;
		screenPosition.y() += moveNext.y() * tile_height / 20;

		//make the shadow move during movement
		shadow.setPos(screenPosition.x() - (tile_width - sprite_width) / 2, screenPosition.y() - tile_height / 2 + sprite_height);

}

void Player::incrementMovement() {

	position += moveNext;

	//nothing left
	if (path.size() <= 0) return;

	ScreenCoord next = path[0];
	path.erase(path.begin());

	moveNext = ScreenCoord(next.x() - position.x(), next.y() - position.y());

	calculateScreenPosition();

}

std::vector<ScreenCoord> Player::getPossibleMoves(Combat& combat) {
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

			std::vector<ScreenCoord> successors = getValidNeighbours(end_position, combat);
			for (ScreenCoord succ : successors) {
				std::vector<ScreenCoord> s(n);
				s.push_back(succ);
				open.push_back(s);
			}
		}
	}

	return seen;
}

void Player::updatePossibleMoves(Combat & combat)
{
	possibleMoves = getPossibleMoves(combat);
}

std::vector<ScreenCoord> Player::getPath(Combat& combat, ScreenCoord to) {
	std::vector<std::vector<ScreenCoord>> open;

	std::vector<ScreenCoord> root;
	root.push_back(position);

	open.push_back(root);
	while (!(open.empty())) {
		std::vector<ScreenCoord> n = heuristic(&open);
		//std::vector<ScreenCoord> n = open[0];
		//open.erase(*node);
		std::vector<std::vector<ScreenCoord>>::iterator index = std::find(open.begin(), open.end(), n);
		open.erase(index);

		if (n.size() <= static_cast<unsigned int>(getMoveSpeed() + 1)) {
			//continue;

			ScreenCoord end_position = n.back();
			if (end_position.x() == to.x() && end_position.y() == to.y()) {
				//n is solution
				return n;
			}

			std::vector<ScreenCoord> successors = getValidNeighbours(end_position, combat);
			for (ScreenCoord succ : successors) {
				std::vector<ScreenCoord> s(n);
				s.push_back(succ);
				open.push_back(s);
			}
		}
	}

	std::vector<ScreenCoord> result;
	return result;
	//no solution found
}

void Player::takeDamageCallback(int damage) {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::TAKE_DAMAGE));
	// Decide what the next animation is based on whether the player is still alive or not
	if (health <= 0) player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::DEAD));
	else player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
}

#include <iostream>
void Player::selectCallback() {
	std::cout << "TEST" << std::endl;
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::SELECTED));
}

std::vector<ScreenCoord> Player::heuristic(std::vector<std::vector<ScreenCoord>> * open) {
	std::vector<ScreenCoord> smallest;
	int smallest_dist = 99999;
	for (std::vector<ScreenCoord> node : *open) {
		ScreenCoord end_position = node.back();
		int x_diff = std::abs(end_position.x() - moveTarget.x());
		int y_diff = std::abs(end_position.y() - moveTarget.y());
		ScreenCoord v(x_diff, y_diff);
		if (v.norm() < smallest_dist) {
			smallest_dist = v.norm();
			smallest = node;
		}
	}

	return smallest;
}

std::vector<ScreenCoord> Player::getValidNeighbours(ScreenCoord pos, Combat& combat) {

	std::vector<ScreenCoord> neighbours;
	ScreenCoord right(pos.x() + 1, pos.y());
	ScreenCoord left(pos.x() - 1, pos.y());
	ScreenCoord top(pos.x(), pos.y() - 1);
	ScreenCoord bot(pos.x(), pos.y() + 1);

	if (pos.x() > 0) {
		if (combat.isPosEmpty(left)) {
			neighbours.push_back(left);
		}
	}

	if (pos.x() < combat.grid.map_width) {
		if (combat.isPosEmpty(right)) {
			neighbours.push_back(right);
		}
	}

	if (pos.y() > 0) {
		if (combat.isPosEmpty(bot)) {
			neighbours.push_back(bot);
		}
	}

	if (pos.y() < combat.grid.map_height) {
		if (combat.isPosEmpty(top)) {
			neighbours.push_back(top);
		}
	}
	return neighbours;
}