#pragma once

#include <GL/glew.h>

class IndexBuffer {

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
private:
	// This represents the index buffer id in openGL
	GLuint bufferID;
	unsigned int count;
};