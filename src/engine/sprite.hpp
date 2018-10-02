#pragma once

#include "opengl/glwrappers.hpp"

class Sprite {

public:

	Sprite(const std::string& path);
	~Sprite();

	void render();

	// Sprite position
	int x, y;
	// Sprite size
	int w, h;

	// helper methods to make life easier
	void setPos(int x, int y);
	void setSize(int w, int h);

private:

	Texture texture;

};