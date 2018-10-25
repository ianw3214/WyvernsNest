#include "player.hpp"

Player::Player() :
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	idle("res/assets/HeroF_Sprite.png")
{
	idle.setSize(sprite_width, sprite_height);
}

Player::Player(int x, int y) :
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
	if (state != 0) {
		switch (state)
		{
		case MOVE_STATE: 
			if (state_counter < 100) {
				state_counter++;

				calculateScreenPositionMovement();
			}
			else {
				state_counter = 0;
				state = 0;
				position = moveTarget;
				calculateScreenPosition();
			}
			break;
		default:
			break;
		}
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

		//position = to;

		moveTarget = to;
		moveDiff = position - to;
		state = MOVE_STATE;
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
	screenPosition.y() = Core::windowHeight() - (position.y() + 1) * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}

void Player::calculateScreenPositionMovement() {
	screenPosition.x() -= moveDiff.x() * 2.13f;
	screenPosition.y() += moveDiff.y() * 1.8f;

}