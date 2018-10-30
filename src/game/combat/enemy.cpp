#include "enemy.hpp"

Enemy::Enemy() :
	Unit(UnitType::ENEMY),
	sprite("res/assets/WyvernFighter_Sprite.png")
{
	sprite.setSize(sprite_width, sprite_height);

	// TEMPORARY DEBUG CODE
	health = 10;
	maxHealth = 10;
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
	int healthLeft = lerp(100, 0, static_cast<float>(health) / static_cast<float>(maxHealth));
	for (int i = 0; i < 5; i++) {
		Core::Renderer::drawLine(pos + ScreenCoord(-50 + health, -50 + i), pos + ScreenCoord(50, -50 + i), Colour(1.0f, 0.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(-50, -50 + i), pos + ScreenCoord(50 - healthLeft, -50 + i), Colour(0.0f, 1.0f, 0.0f));
	}

}

void Enemy::update()
{
}