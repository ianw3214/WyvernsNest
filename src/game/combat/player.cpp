#include "player.hpp"

Player::Player() :
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	idle("res/assets/HeroF_Sprite.png")
{
	idle.setSize(sprite_width, sprite_height);
}

Player::~Player()
{
}

void Player::render()
{
	if (selected) {
		Sprite sprite("res/test5.png");
		sprite.setPos(screenPosition.x(), screenPosition.y());
		sprite.setSize(sprite_width, sprite_height);
		sprite.render();
	}
	else {
		idle.setPos(screenPosition.x(), screenPosition.y());
		idle.render();
	}


}

void Player::update()
{
}

void Player::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	calculateScreenPosition();
}

void Player::move(Vec2<int> to)
{
	position = to;
	calculateScreenPosition();
}

void Player::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	screenPosition.y() = Core::windowHeight() - (position.y() + 1) * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}
