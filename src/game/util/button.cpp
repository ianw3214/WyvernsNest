#include "button.hpp"

ButtonData::ButtonData(Vec2<int> position, int width, int height) :
	default_sprite("res/assets/UI/SkillTreeLink.png"),
	hover_sprite("res/assets/UI/SkillTreeLinkHover.png"),
	position(position),
	width(width),
	height(height)
{
	default_sprite.setSize(width, height);
	default_sprite.setPos(position.x(), position.y());
	hover_sprite.setSize(width, height);
	hover_sprite.setPos(position.x(), position.y());
}

ButtonData::~ButtonData() {

}

void ButtonData::setSprites(const std::string & defaultSpr, const std::string & hoverSpr, const std::string & pressedSpr) {
	default_sprite = Sprite(defaultSpr);
	default_sprite.setSize(width, height);
	default_sprite.setPos(position.x(), position.y());
	hover_sprite = Sprite(hoverSpr);
	hover_sprite.setSize(width, height);
	hover_sprite.setPos(position.x(), position.y());
}

void ButtonData::render() {
	ScreenCoord mousePos;
	SDL_GetMouseState(&mousePos.x(), &mousePos.y());
	if (colliding(mousePos)) hover_sprite.render();
	else default_sprite.render();
}

bool ButtonData::colliding(ScreenCoord point) {
	if (point.x() >= position.x() && point.x() <= position.x() + width &&
		point.y() >= position.y() && point.y() <= position.y() + height)
	{
		return true;
	}
	return false;
}
