#pragma once

#include "opengl/glwrappers.hpp"

class Sprite {

public:

	Sprite(const std::string& path);
	~Sprite();

	void render(int x, int y);
	void render(int x, int y, int w, int h);

private:
	// Sprite size
	int w, h;
	Texture texture;

};