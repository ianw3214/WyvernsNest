#include "player.hpp"

Player::Player() :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	state_counter(0),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
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
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
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
	if (selected) {
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
				current_action = PlayerAction::MOVE;
			}
			// Attack key
			if (event.key.keysym.sym == SDLK_KP_2) {

				attack1.playerPos = position;
				attack1.toggleRender();
				current_action = PlayerAction::ATTACK_1;
			}
			if (event.key.keysym.sym == SDLK_KP_3) {

				attack2.playerPos = position;
				attack2.toggleRender();
				current_action = PlayerAction::ATTACK_2;
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
					state = UnitState::IDLE;
					position = moveTarget;
					calculateScreenPosition();
				}
			}
		} break;
		default: {
			// Do nothing by default
		} break;
	}
}

void Player::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	calculateScreenPosition();
}

std::vector<ScreenCoord> Player::click(Vec2<int> to)
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
			return (attack1.getAttackPos(to));
		} break;
		case PlayerAction::ATTACK_2: {
			// do the action here
			turnfOffAttacks();
			return (attack2.getAttackPos(to));
		} break;
		default: {
			// do nothing
		} break;
	}

	std::vector<ScreenCoord>result;
	return result;
}

void Player::turnfOffAttacks()
{
	attack1.isRendered = false;
	attack2.isRendered = false;
	//do the same for all attacks
}

void Player::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	//screenPosition.y() = Core::windowHeight() - (position.y() + 1) * tile_height;
	screenPosition.y() =  position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}

void Player::calculateScreenPositionMovement() {
		screenPosition.x() += moveNext.x() * 230 / 20;
		screenPosition.y() += moveNext.y() * 200 / 20;
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