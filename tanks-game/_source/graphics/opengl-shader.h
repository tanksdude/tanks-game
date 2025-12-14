#pragma once
#include "shader.h"
#include <vector>
#include <unordered_map>
#include <glad/gl.h>

class OpenGLShader final : public Shader {
protected:
	static void read_shader_src(const char* fname, std::vector<char>& buffer);
	static GLuint load_and_compile_shader(const char* fname, GLenum shaderType);
	static GLuint create_program(const char* path_vert_shader, const char* path_frag_shader);

	std::unordered_map<std::string, int> uniformLocationCache;
	int getUniformLocation(const std::string& name);

public:
	void setUniform1i(const std::string& name, int i) override;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) override;
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix) override;

	void Bind() const override;
	void Unbind() const override;

	OpenGLShader(const std::string& vert_filepath, const std::string& frag_filepath);
	~OpenGLShader();
};
