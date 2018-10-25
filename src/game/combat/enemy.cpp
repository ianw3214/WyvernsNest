#include "enemy.hpp"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::render()
{
	//Sprite sprite("res/test5.png");
	//sprite.setPos(100 * position.x() + (position.x() + 1) * 10 + (50 - width / 2), 620 - 100 * position.y() - (position.y() + 1) * 10 + (50 - height / 2));
	//sprite.setSize(width, height);
	//sprite.render();
}

void Enemy::update()
{
}

void Enemy::takeDamage(int dmg)
{
	health -= dmg;

	if (health <= 0) {
		//die
	}
}

ScreenCoord Enemy::getPosition()
{
	return position;
}