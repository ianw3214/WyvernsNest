#pragma once

#include <GL/glew.h>

#include <string>

class Texture {

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
private:
	GLuint textureID;
	std::string filePath;
	unsigned char * localBuffer;
	int width, height, bitsPerPixel;
};