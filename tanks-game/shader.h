#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glew.h> //feels bad including this just for #define stuff, but files that need this file will already be including glew
#include "res/vendor/glm/glm.hpp"

class Shader {
private:
	std::string vert_filepath;
	std::string frag_filepath;
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;

	int getUniformLocation(const std::string& name);

	static void read_shader_src(const char* fname, std::vector<char>& buffer);
	static GLuint load_and_compile_shader(const char* fname, GLenum shaderType);
	static GLuint create_program(const char* path_vert_shader, const char* path_frag_shader);

public:
	Shader(const std::string& vert_filepath, const std::string& frag_filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	unsigned int getRendererID() const { return rendererID; }
};