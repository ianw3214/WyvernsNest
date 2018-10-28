#include "player.hpp"

Player::Player() :
	Unit(UnitType::PLAYER),
	state_counter(0),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	idle("res/assets/HeroF_Sprite.png")
{
	idle.setSize(sprite_width, sprite_height);
}

Player::Player(int x, int y) :
	Unit(UnitType::PLAYER),
	state_counter(0),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	idle("res/assets/HeroF_Sprite.png")
{
	idle.setSize(sprite_width, sprite_height);
	position.x() = x;
	position.y() = y;
}

Player::~Player()
{
}

void Player::render()
{
	if (!selected) {
		Sprite sprite("res/assets/HeroF_Sprite.png");
		sprite.setPos(screenPosition.x(), screenPosition.y());
		sprite.setSize(sprite_width, sprite_height);
		sprite.render();
	}
	else {
		Sprite sprite("res/HeroF_Sprite_Selected.png");
		sprite.setPos(screenPosition.x(), screenPosition.y());
		sprite.setSize(sprite_width, sprite_height);
		sprite.render();
	}

	//TODO doesnt work if we have two players in a vector

	//if (!selected) {
	//	Sprite sprite("res/test5.png");
	//	sprite.setPos(screenPosition.x(), screenPosition.y());
	//	sprite.setSize(sprite_width, sprite_height);
	//	sprite.render();
	//}
	//else {
	//	idle.setPos(screenPosition.x(), screenPosition.y());
	//	idle.render();
	//}

}

void Player::handleEvent(const SDL_Event & event)
{
	if (selected) {
		SDL_KeyboardEvent test = event.key;
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == 1073741913) {
				attack1.playerPos = position;
				attack1.toggleRender();

				if (attackIndex == 1) {

					attackIndex = 0;
				}
				else {
					attackIndex = 1;
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
			if (state_counter < 100) {
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

ScreenCoord Player::move(Vec2<int> to)
{
	if (attackIndex == 0) {
		moveTarget = to;
		moveNext = ScreenCoord(0, 0);
		incrementMovement();
		state = UnitState::MOVE;
	}
	else {
		switch (attackIndex)
		{
		case 1: 
			attackIndex = 0;
			return attack1.attack(to);
		default:
			break;
		}
	}


	return to;

}

void Player::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	//screenPosition.y() = Core::windowHeight() - (position.y() + 1) * tile_height;
	screenPosition.y() =  position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}

void Player::calculateScreenPositionMovement() {
		screenPosition.x() += moveNext.x() * 230 / 100;
		screenPosition.y() += moveNext.y() * 200 / 100;
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