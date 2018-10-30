#include "sprite.hpp"

#include "core.hpp"

Sprite::Sprite(const std::string & path) : 
	texture(path),
	x(0), y(0),
	w(0), h(0),
	src_x(0), src_y(0),
	src_w(0), src_h(0),
	original_w(0),
	original_h(0)
{
	src_w = original_w = w = texture.getWidth();
	src_h = original_h = h = texture.getHeight();
}

Sprite::~Sprite() {

}

void Sprite::render() {
	// Core::Renderer::drawTexture(ScreenCoord(x, y), w, h, texture);
	Core::Renderer::drawSprite(*this);
}

void Sprite::setPos(int x, int y) {
	this->x = x;
	this->y = y;
}

void Sprite::setSize(int w, int h) {
	this->w = w;
	this->h = h;
}

void Sprite::setSourcePos(int x, int y) {
	this->src_x = x;
	this->src_y = y;
}

void Sprite::setSourceSize(int w, int h) {
	this->src_w = w;
	this->src_h = h;
}
