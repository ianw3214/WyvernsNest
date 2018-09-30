#include "vertexBuffer.hpp"

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