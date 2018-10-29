#include "enemy.hpp"

Enemy::Enemy() :
	Unit(UnitType::ENEMY),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	sprite("res/assets/WyvernFighter_Sprite.png")
{
	sprite.setSize(sprite_width, sprite_height);

}

Enemy::~Enemy()
{
}

void Enemy::render()
{
	if (!isDead) {
		sprite.setPos(screenPosition.x(), screenPosition.y());
		sprite.render();
	}

	drawHealth();

}

void Enemy::drawHealth() {
	ScreenCoord pos = screenPosition + ScreenCoord((tile_width - sprite_width) / 2, (tile_height - sprite_height) / 2);
	double healthLeft = (1 - double(health) / maxHealth) * 100;
	for (int i = 0; i < 5; i++) {
		Core::Renderer::drawLine(pos + ScreenCoord(-50 + health, -50 + i), pos + ScreenCoord(50, -50 + i), Colour(1.0, 0.0, 0.0));
		Core::Renderer::drawLine(pos + ScreenCoord(-50, -50 + i), pos + ScreenCoord(50 - healthLeft, -50 + i), Colour(0.0, 1.0, 0.0));
	}

}

void Enemy::update()
{
}

void Enemy::takeDamage(int dmg)
{
	health -= dmg;

	if (health <= 0) {
		//die
		isDead = true;
	}
}

ScreenCoord Enemy::getPosition()
{
	return position;
}

void Enemy::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	screenPosition.y() = position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() += (tile_height - sprite_height) / 2;
}

void Enemy::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	calculateScreenPosition();
}