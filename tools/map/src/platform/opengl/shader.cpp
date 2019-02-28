#include "shader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertex, const std::string& fragment)
	: vertexFilePath(vertex), fragmentFilePath(fragment), rendererID(0)
{
	rendererID = createShaderFromFile(vertex, fragment);
}

Shader::~Shader() {
	glDeleteProgram(rendererID);
}

void Shader::bind() const {
	glUseProgram(rendererID);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string & name, int value) {
	bind();
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform3f(const std::string& name, glm::vec3 vec) {
	bind();
	glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
	bind();
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, glm::vec4 vec) {
	bind();
	glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}


void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3) {
	bind();
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string & name, const glm::mat4 & matrix) {
	bind();
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

GLint Shader::getUniformLocation(const std::string & name) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}
	unsigned int location = glGetUniformLocation(rendererID, name.c_str());
	if (location < 0) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	uniformLocationCache[name] = location;
	return location;
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
	GLuint id = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Handle errors if the shader compilation failed
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Shader compilation failed:\n" << message << std::endl;
		}
	}

	return id;
}

GLuint Shader::createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {

	GLuint program = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// TODO: assert that shaders were compiled successfully
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			GLchar * message = (GLchar*)alloca(length * sizeof(GLchar));
			glGetProgramInfoLog(program, length, &length, message);
			std::cout << "Program link failed:\n" << message << std::endl;
		}
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	// TODO: Error handling

	// Delete the shaders once the program has been created
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;

}

GLuint Shader::createShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath) {

	std::stringstream vertexSource;
	std::stringstream fragmentSource;

	{	// Get the vertex shader source code
		std::ifstream vertexFile(vertexPath);

		if (!vertexFile.is_open()) {
			// TODO: Error handling
		}

		std::string line;
		while (std::getline(vertexFile, line)) {
			vertexSource << line << '\n';
		}
	}

	// Get the fragment shader source code
	{
		std::ifstream fragmentFile(fragmentPath);

		if(!fragmentFile.is_open()) {
			// TODO: Error handling
		}

		std::string line;
		while (std::getline(fragmentFile, line)) {
			fragmentSource << line << '\n';
		}
	}

	return createShaderProgram(vertexSource.str(), fragmentSource.str());

}