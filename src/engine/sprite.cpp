#include "sprite.hpp"

#include "core.hpp"

Sprite::Sprite(const std::string & path) : 
	texture(path),
	x(0), y(0),
	w(0), h(0),
	src_x(0), src_y(0),
	src_w(0), src_h(0),
	original_w(0),
	original_h(0),
	modelMatrix{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f },
	texMatrix{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f }
{
	src_w = original_w = w = Core::getTexture(path)->getWidth();
	src_h = original_h = h = Core::getTexture(path)->getHeight();

	updateModelMatrix();
	updateTexMatrix();
}

Sprite::~Sprite() {
	
}

void Sprite::render() {
	// Core::Renderer::drawTexture(ScreenCoord(x, y), w, h, texture);
	Core::Renderer::drawSprite(*this);
}

const Texture & Sprite::getTexture() const {
	return *Core::getTexture(texture);
}

void Sprite::setPos(int x, int y) {
	this->x = x;
	this->y = y;

	updateModelMatrix();
}

void Sprite::setSize(int w, int h) {
	this->w = w;
	this->h = h;

	updateModelMatrix();
}

void Sprite::setSourcePos(int x, int y) {
	this->src_x = x;
	this->src_y = y;

	updateTexMatrix();
}

void Sprite::setSourceSize(int w, int h) {
	this->src_w = w;
	this->src_h = h;

	updateTexMatrix();
}

std::array<float, 16> Sprite::getModelMatrix() const {
	return modelMatrix;
}

std::array<float, 16> Sprite::getTexMatrix() const {
	return texMatrix;
}

void Sprite::updateModelMatrix() {
	modelMatrix[0] = static_cast<float>(w);
	modelMatrix[3] = static_cast<float>(x);
	modelMatrix[5] = static_cast<float>(-h);
	modelMatrix[7] = static_cast<float>(Engine::get_instance().getWindowHeight()) - static_cast<float>(y);
}

void Sprite::updateTexMatrix() {
	texMatrix[0] = static_cast<float>(src_w)/static_cast<float>(original_w);
	texMatrix[3] = static_cast<float>(src_x)/static_cast<float>(original_w);
	texMatrix[5] = -static_cast<float>(src_h)/static_cast<float>(original_h);
	texMatrix[7] = static_cast<float>(src_y + src_h)/static_cast<float>(original_h);
}
