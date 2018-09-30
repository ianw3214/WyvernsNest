#pragma once

#include <GL/glew.h>

#include "opengl/glwrappers.hpp"
#include "../math/math.hpp"

class Renderer {

public:
	Renderer();

	void clear() const;
	void drawLine(ScreenCoord v1, ScreenCoord v2, Colour colour);
	void drawTexture(ScreenCoord v, int width, int height, const Texture& texture);
private:
	void drawTriangles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void drawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void drawLineStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum type) const;

	// static shaders for sprite and solid fill drawing
	Shader basicShader;
	Shader textureShader;
};