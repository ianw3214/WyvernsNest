#pragma once

#include <string>

#include <unordered_map>

#include <GL/glew.h>

class Shader {

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform2i(const std::string & name, int v0, int v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string vertexSource, fragmentSource;
	GLuint rendererID;

	// caching for uniforms
	std::unordered_map<std::string, GLint> uniformLocationCache;

	GLint getUniformLocation(const std::string& name);
	// Shader compilation functions
	GLuint compileShader(GLenum type, const std::string& source);
	std::string getShaderSrc(const std::string& filePath);
	GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
};