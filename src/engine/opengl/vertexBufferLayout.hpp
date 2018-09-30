#pragma once

#include <GL/glew.h>

#include <vector>

struct VertexBufferLayoutElement {
	GLenum type;
	unsigned int count;
	GLboolean normalized;

	static unsigned int getSizeOfType(GLenum type) {
		switch (type) {
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
		}
		// Notify any unsupported types
		// ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {

public:

	VertexBufferLayout() : stride(0) {}

	// Getter methods
	inline const std::vector<VertexBufferLayoutElement> getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }

	// Templated function to add attributes to the layout
	template<typename T>
	void push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

private:
	std::vector<VertexBufferLayoutElement> elements;
	unsigned int stride;

};