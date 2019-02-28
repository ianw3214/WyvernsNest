#include "glwrappers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &bufferID);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void VertexBuffer::unBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count) : count(count) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray() {
	glGenVertexArrays(1, &arrayID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout, unsigned int index) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = index; i < elements.size() + index; ++i) {
		const auto& element = elements[i - index];
		glEnableVertexAttribArray(i);
		if (element.type == GL_INT) {
			glVertexAttribIPointer(i, element.count, element.type, layout.getStride(), (const void*)offset);
		} else {
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		}
		offset += element.count * VertexBufferLayoutElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(arrayID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

Texture::Texture(const std::string & path)
	: textureID(0), filePath(path), localBuffer(nullptr),
	width(0), height(0), bitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

void Texture::bind(unsigned int slot /*= 0*/) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}