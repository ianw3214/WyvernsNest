#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

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

    void push_float(unsigned int count) {
        elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_FLOAT) * count;
    }
    void push_uint(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}
	void push_uchar(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	void push_int(unsigned int count) {
		elements.push_back({ GL_INT, count, GL_TRUE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_INT) * count;
	}
private:
	std::vector<VertexBufferLayoutElement> elements;
	unsigned int stride;

};

class VertexBuffer {
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unBind() const;
private:
	// This represents the vertex buffer id in openGL
	GLuint bufferID;
};

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

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer & vb, const VertexBufferLayout& layout, unsigned int index = 0);

	void bind() const;
	void unbind() const;
private:
	GLuint arrayID;
};

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