#include "player.hpp"

Player::Player()
{
	m_test4 = ResourceEngine::getSprite("test4");
	m_test5 = ResourceEngine::getSprite("test5");
}

Player::~Player()
{
}

void Player::render()
{
	if (selected) {
		m_test5->render(100 * position.x() + (position.x() + 1) * 10 + (50 - width / 2), 620 - 100 * position.y() - (position.y() + 1) * 10 + (50 - height / 2), width, height);
	}
	else {
		m_test4->render(100 * position.x() + (position.x() + 1) * 10 + (50 - width / 2), 620 - 100 * position.y() - (position.y() + 1) * 10 + (50 - height / 2), width, height);
	}


}

void Player::update()
{
}

void Player::move(ScreenCoord to)
{
	position = to;
}
