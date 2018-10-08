#pragma once

#include <string>

#include <unordered_map>

#include <GL/glew.h>

class Shader {

public:
	Shader(const std::string& vertex, const std::string& fragment);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	GLuint rendererID;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string vertexSource, fragmentSource;
	// caching for uniforms
	std::unordered_map<std::string, GLint> uniformLocationCache;

	GLint getUniformLocation(const std::string& name);
	// Shader compilation functions
	GLuint compileShader(GLenum type, const std::string& source);
	GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
};