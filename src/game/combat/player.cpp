#include "player.hpp"
#include "../combat.hpp"

Player::Player() :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	state_counter(0),
	sprite_idle("res/assets/HeroF_Sprite.png"),
	sprite_selected("res/HeroF_Sprite_Selected.png")
{
	sprite_idle.setSize(sprite_width, sprite_height);
	sprite_selected.setSize(sprite_width, sprite_height);
}

Player::Player(int x, int y) :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	state_counter(0),
	sprite_idle("res/assets/HeroF_Sprite.png"),
	sprite_selected("res/HeroF_Sprite_Selected.png")
{
	sprite_idle.setSize(sprite_width, sprite_height);
	sprite_selected.setSize(sprite_width, sprite_height);
	position.x() = x;
	position.y() = y;
}

Player::~Player()
{
}

void Player::render()
{
	// Render the shadow first
	shadow.render();

	if (selected) {
		if (current_action == PlayerAction::ATTACK_1) {
			attack1.renderValidGrid();
		}
		if (current_action == PlayerAction::ATTACK_2) {
			attack2.renderValidGrid();
		}
		sprite_selected.setPos(screenPosition.x(), screenPosition.y());
		sprite_selected.render();
	}
	else {
		sprite_idle.setPos(screenPosition.x(), screenPosition.y());
		sprite_idle.render();
	}

}
void Player::handleEvent(const SDL_Event & event)
{

	// Only handle events for the entity if it is selected
	if (selected) {
		if (event.type == SDL_KEYDOWN) {
			// Move Key
			if (event.key.keysym.sym == SDLK_KP_1) {
				if (state == UnitState::IDLE) {
					current_action = PlayerAction::MOVE;
				}
			}
			// Attack 1 key
			if (event.key.keysym.sym == SDLK_KP_2) {
				if (state == UnitState::IDLE || state == UnitState::ATTACK) {
					current_action = PlayerAction::ATTACK_1;
				}
			}
			// Attack 2 key
			if (event.key.keysym.sym == SDLK_KP_3) {
				if (state == UnitState::IDLE || state == UnitState::ATTACK) {
					current_action = PlayerAction::ATTACK_2;
				}
			}
		}
		
	}
}

void Player::update(int delta)
{
	// Update the player based on its current state	
	switch (state) {
		case UnitState::IDLE: {
			// Do nothing when idling
		} break;
		case UnitState::MOVE: {
			// Move the player towards its destination
			if (state_counter < 20) {
				state_counter++;
				calculateScreenPositionMovement();
			}
			else {
				state_counter = 0;
				incrementMovement();
				// If the player reaches the target destination, stop moving it
				if (position.x() == moveTarget.x() && position.y() == moveTarget.y()) {
					state = UnitState::ATTACK;
					current_action = PlayerAction::NONE;
					position = moveTarget;
					calculateScreenPosition();
				}
			}
		} break;
		case UnitState::ATTACK: {

		} break;
		default: {
			// Do nothing by default
		} break;
	}
}

void Player::click(Vec2<int> to, Combat& combat)
{
	switch (current_action) {
		case PlayerAction::NONE: {
			// do nothing
		} break;
		case PlayerAction::MOVE: {
			moveTarget = to;
			moveNext = ScreenCoord(0, 0);
			incrementMovement();
			state = UnitState::MOVE;
		} break;
		case PlayerAction::ATTACK_1: {
			// do the action here
			turnfOffAttacks();
			attack1.attack(to, combat);
			current_action = PlayerAction::NONE;
			state = UnitState::DONE;
		} break;
		case PlayerAction::ATTACK_2: {
			// do the action here
			turnfOffAttacks();
			attack2.attack(to, combat);
			current_action = PlayerAction::NONE;
			state = UnitState::DONE;
		} break;
		default: {
			// do nothing
		} break;
	}
}

void Player::turnfOffAttacks()
{
	current_action = PlayerAction::NONE;
	//state = UnitState::IDLE;
	//do the same for all attacks
}

void Player::calculateScreenPositionMovement() {
		screenPosition.x() += moveNext.x() * tile_width / 20;
		screenPosition.y() += moveNext.y() * tile_height / 20;

		//make the shadow move during movement
		shadow.setPos(screenPosition.x() - (tile_width - sprite_width) / 2, screenPosition.y() - tile_height / 2 + sprite_height);

}

void Player::incrementMovement() {

	position += moveNext;

	calculateScreenPosition();

	if (position.x() != moveTarget.x()) {
		if (position.x() < moveTarget.x()) {
			moveNext = ScreenCoord(1, 0);
		}
		else {
			moveNext = ScreenCoord(-1, 0);
		}
	}
	else if (position.y() != moveTarget.y()) {
		if (position.y() < moveTarget.y()) {
			moveNext = ScreenCoord(0, 1);
		}
		else {
			moveNext = ScreenCoord(0, -1);
		}
	}
}

void Player::getPath(Combat& combat) {
	std::vector<std::vector<ScreenCoord>> open;

	std::vector<ScreenCoord> root;
	root.push_back(position);

	open.push_back(root);
	while (!(open.empty())) {
		std::vector<ScreenCoord> n = open[0];
		open.erase(open.begin());

		ScreenCoord end_position = n.back();
		if (end_position.x() == moveTarget.x() && end_position.y() == moveTarget.y()) {
			//n is solution
		}

		std::vector<ScreenCoord> successors = getValidNeighbours(end_position, combat);
		for (ScreenCoord succ : successors) {
			std::vector<ScreenCoord> s(n);
			s.push_back(succ);
		}
	}
	//no solution found
}

std::vector<ScreenCoord> Player::getValidNeighbours(ScreenCoord pos, Combat& combat) {

	std::vector<ScreenCoord> neighbours;
	ScreenCoord right(pos.x() + 1, pos.y());
	ScreenCoord left(pos.x() - 1, pos.y());
	ScreenCoord top(pos.x(), pos.y() - 1);
	ScreenCoord bot(pos.x(), pos.y() + 1);

	if (pos.x() > 0) {
		if (combat.grid.isPosValid(right)) {
			neighbours.push_back(right);
		}
	}

	if (pos.x() < combat.grid.map_width) {
		if (combat.grid.isPosValid(left)) {
			neighbours.push_back(left);
		}
	}

	if (pos.y() > 0) {
		if (combat.grid.isPosValid(bot)) {
			neighbours.push_back(bot);
		}
	}

	if (pos.y() < combat.grid.map_height) {
		if (combat.grid.isPosValid(top)) {
			neighbours.push_back(top);
		}
	}

	return neighbours;
}