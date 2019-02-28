#pragma once

#include <string>

#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {

public:
	Shader(const std::string& vertex, const std::string& fragment);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform3f(const std::string& name, glm::vec3 vec);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, glm::vec4 vec);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	std::string vertexFilePath, fragmentFilePath;
	GLuint rendererID;
	// caching for uniforms
	std::unordered_map<std::string, GLint> uniformLocationCache;

	GLint getUniformLocation(const std::string& name);
	// Shader compilation functions
	GLuint compileShader(GLenum type, const std::string& source);
	GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
	GLuint createShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
};