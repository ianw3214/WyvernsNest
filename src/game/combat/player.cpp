#include "player.hpp"

Player::Player()
{

}

Player::~Player()
{
}

void Player::render()
{
	if (selected) {
		Sprite sprite("res/test5.png");
		sprite.setPos(100 * position.x() + (position.x() + 1) * 10 + (50 - width / 2), 620 - 100 * position.y() - (position.y() + 1) * 10 + (50 - height / 2));
		sprite.setSize(width, height);
		sprite.render();
	}
	else {
		Sprite sprite("res/test4.png");
		sprite.setPos(100 * position.x() + (position.x() + 1) * 10 + (50 - width / 2), 620 - 100 * position.y() - (position.y() + 1) * 10 + (50 - height / 2));
		sprite.setSize(width, height);
		sprite.render();
	}


}

void Player::update()
{
}

void Player::move(ScreenCoord to)
{
	position = to;
}
