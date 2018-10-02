#include "sprite.hpp"

#include "core.hpp"

Sprite::Sprite(const std::string & path) : 
	texture(path),
	x(0), y(0),
	w(0), h(0)
{
	w = texture.getWidth();
	h = texture.getHeight();
}

Sprite::~Sprite() {

}

void Sprite::render() {
	Core::Renderer::drawTexture(ScreenCoord(x, y), w, h, texture);
}

void Sprite::setPos(int x, int y) {
	this->x = x;
	this->y = y;
}

void Sprite::setSize(int w, int h) {
	this->w = w;
	this->h = h;
}
