#pragma once

#include <GL/glew.h>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include <string>
#include <unordered_map>

class Renderer {
private:
	static std::unordered_map<std::string, Shader*> shaderCache;
	static unsigned int currentShader;
	static unsigned int currentVertexArray;
	static unsigned int currentIndexBuffer;
	static inline void bindShader(Shader*);
	static inline void bindShader(const Shader&);
	static inline void bindVertexArray(const VertexArray&);
	static inline void bindIndexBuffer(const IndexBuffer&);
public:
	static Shader* getShader(std::string);
	static void Clear();
	static void Clear(int bits);
	static void Initialize();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void Draw(const VertexArray& va, const Shader& shader, GLenum type, GLint first, GLsizei count);
	static void Draw(GLenum type, GLint first, GLsizei count);
	static void Unbind(const VertexArray&);
	static void Unbind(const IndexBuffer&);
	static void Unbind(const Shader&);
	static void UnbindAll();
	static void Cleanup();
};