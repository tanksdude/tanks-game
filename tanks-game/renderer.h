#pragma once

#include <GL/glew.h>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

class Renderer {
public:
	static void Clear();
	static void Clear(int bits);
	static void Initialize();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void Cleanup();
};