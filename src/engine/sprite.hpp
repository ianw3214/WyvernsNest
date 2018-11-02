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

	const Texture& getTexture() const;

	// Texture source information
	int src_x, src_y;
	int src_w, src_h;

	// This should never be touched
	// TODO: FIND A MORE ELEGANT SOLUTION FOR THIS
	int original_w, original_h;

	// helper methods to make life easier
	void setPos(int x, int y);
	void setSize(int w, int h);
	void setSourcePos(int x, int y);
	void setSourceSize(int w, int h);

private:

	std::string texture;

};