#include "shader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: rendererID(0)
{
	std::string vertex = getShaderSrc(vertexPath);
	std::string fragment = getShaderSrc(fragmentPath);
	rendererID = createShaderProgram(vertex, fragment);
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

void Shader::setUniform2i(const std::string & name, int v0, int v1) {
	bind();
	glUniform2i(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
	bind();
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3) {
	bind();
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
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

std::string Shader::getShaderSrc(const std::string& filePath) {
	std::string shaderSrc;
    std::ifstream fileSrc;
    fileSrc.open(filePath);
    if (fileSrc.is_open()) {
        std::string line;
        while (std::getline(fileSrc, line)) shaderSrc += "\n" + line;
        fileSrc.close();
    } else {
        std::cout << "Could not read shader: " << filePath << std::endl;
    }
	return shaderSrc;
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