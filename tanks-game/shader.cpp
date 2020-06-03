#pragma once

#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

Shader::Shader(const std::string& vert_filepath, const std::string& frag_filepath) {
	//uniformLocationCache; //I don't know if it needs to be initialized
	this->vert_filepath = vert_filepath;
	this->frag_filepath = frag_filepath;

	rendererID = create_program(vert_filepath.c_str(), frag_filepath.c_str());
}

Shader::~Shader() {
	glDeleteProgram(rendererID);
}

//from Solarian Programmer:
// Read a shader source from a file
// store the shader source in a std::vector<char>
void Shader::read_shader_src(const char* fname, std::vector<char>& buffer) {
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if(in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}
// Compile a shader
GLuint Shader::load_and_compile_shader(const char* fname, GLenum shaderType) {
	// Load a shader from an external file
	std::vector<char> buffer;
	read_shader_src(fname, buffer);
	const char* src = &buffer[0];

	// Compile the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	// Check the result of the compilation
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	return shader;
}
// Create a program from two shaders
GLuint Shader::create_program(const char* path_vert_shader, const char* path_frag_shader) {
	// Load and compile the vertex and fragment shaders
	GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
	GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);
	//glUseProgram(shaderProgram); //don't use

	return shaderProgram;
}
//end Solarian Programmer code

void Shader::Bind() const {
	glUseProgram(rendererID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const std::string& name) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}

	int location = glGetUniformLocation(rendererID, name.c_str());
	if (location == -1) {
		std::cout << "Uniform '" << name << "' doesn't exist!" << std::endl;
	}

	uniformLocationCache.insert({ name, location });
	return location;
}
