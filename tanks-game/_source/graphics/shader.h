#pragma once
#include <string>
#include <glm.hpp>

class Shader {
protected:
	std::string vert_filepath;
	std::string frag_filepath;
	unsigned int rendererID;

public:
	virtual void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
	virtual void setUniformMat4f(const std::string& name, const glm::mat4& matrix) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	unsigned int getRendererID() const { return rendererID; }

	static Shader* MakeShader(const std::string& vert_filepath, const std::string& frag_filepath);
	virtual ~Shader() { return; }
};
