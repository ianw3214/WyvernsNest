#include "indexBuffer.hpp"

#include <iostream>

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