#include "renderer.hpp"

#include <algorithm>
#include <array>

#include "engine.hpp"
#include "sprite.hpp"
#include "core.hpp"

// Index orders for different shapes
const unsigned int LINE_INDICES[2] = { 0, 1 };
const unsigned int SQUARE_INDICES[6] = { 0, 1, 3, 0, 2, 3 };

Renderer::Renderer() : 
	basicShader("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl"),
	textureShader("res/shaders/basic_vertex.glsl", "res/shaders/basic_texture.glsl")
{
	initRectangleData();
	initLineData();

}

void Renderer::clear() const {
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawLine_fast(ScreenCoord v1, ScreenCoord v2, Colour colour) {
	basicShader.bind();
	float width = static_cast<float>(v2.x() - v1.x());
	float height = static_cast<float>(v2.y() - v1.y());
	float matrix[16] = {
		width, 0.f, 0.f, static_cast<float>(v1.x()),
		0.f, -height, 0.f, static_cast<float>(Engine::get_instance().getWindowHeight()) - v1.y(),
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	basicShader.setUniform4f("u_Colour", colour.r(), colour.g(), colour.b(), alpha);
	basicShader.setUniformMat4("u_Model", matrix);
	basicShader.setUniformMat4("u_view", camera.getViewMatrix().data());

	glBindVertexArray(lineVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::drawRect_fast(ScreenCoord v, int width, int height, Colour colour) {
	basicShader.bind();
	float matrix[16] = {
		static_cast<float>(width), 0.f, 0.f, static_cast<float>(v.x()),
		0.f, static_cast<float>(-height), 0.f, static_cast<float>(Engine::get_instance().getWindowHeight()) - v.y(),
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	basicShader.setUniform4f("u_Colour", colour.r(), colour.g(), colour.b(), alpha);
	basicShader.setUniformMat4("u_Model", matrix);
	basicShader.setUniformMat4("u_View", camera.getViewMatrix().data());

	glBindVertexArray(rectangleVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::drawSprite_fast(const Sprite & sprite) {
	textureShader.bind();

	textureShader.setUniform2f("screenDimensions", Core::windowWidth(), Core::windowHeight());
	textureShader.setUniformMat4("u_Model", sprite.getModelMatrix().data());
	textureShader.setUniformMat4("u_View", camera.getViewMatrix().data());
	textureShader.setUniformMat4("u_texMap", sprite.getTexMatrix().data());

	sprite.getTexture().bind();

	glBindVertexArray(rectangleVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::setAlpha(float a) {
	alpha = std::fmin(1.f, std::fmax(0.f, a));
}


void Renderer::initRectangleData() {
	glGenVertexArrays(1, &rectangleVAO_);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(rectangleVAO_);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Renderer::initLineData() {
	glGenVertexArrays(1, &lineVAO_);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLfloat lineVertices[] = {
		// Pos
		0.0f, 0.0f,
		1.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

	glBindVertexArray(lineVAO_);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}