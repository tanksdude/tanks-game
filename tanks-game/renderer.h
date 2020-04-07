#pragma once

#include <string>
#include <unordered_map>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include <GL/glew.h>
#include <glm.hpp>

class Renderer {
public:
	//public until I rewrite rendering
	static glm::mat4 proj;
	static glm::mat4 getProj();
	static int window_width;
	static int window_height;

private:
	static std::unordered_map<std::string, Shader*> shaderCache;
	static unsigned int currentShader;
	static unsigned int currentVertexArray;
	static unsigned int currentIndexBuffer;
	static inline void bindShader(Shader*);
	static inline void bindShader(const Shader&);
	static inline void bindVertexArray(const VertexArray&);
	static inline void bindIndexBuffer(const IndexBuffer&);

	static std::string getErrorString(GLenum err);

public:
	static Shader* getShader(std::string);
	static void Clear();
	static void Clear(int bits);
	static void Initialize();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count);
	static void Draw(const VertexArray& va, const Shader& shader, GLenum type, GLint first, GLsizei count);
	static void Draw(GLenum type, GLint first, GLsizei count);
	static glm::mat4 GenerateMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY);
	static void Unbind(const VertexArray&);
	static void Unbind(const IndexBuffer&);
	static void Unbind(const Shader&);
	static void UnbindAll();
	static void Cleanup();

	static void printGLError();
};