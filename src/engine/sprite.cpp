#include "sprite.hpp"

#include "core.hpp"

Sprite::Sprite(const std::string & path) : 
	texture(path),
	w(0), h(0)
{
	w = texture.getWidth();
	h = texture.getHeight();
}

Sprite::~Sprite() {

}

void Sprite::render(int x, int y) {
	Core::Renderer::drawTexture(ScreenCoord(x, y), this->w, this->h, texture);
}

void Sprite::render(int x, int y, int w, int h) {
	Core::Renderer::drawTexture(ScreenCoord(x, y), w, h, texture);
}
