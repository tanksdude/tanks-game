#pragma once

#include "renderer.h"

struct BackgroundRect {
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;

public:
	static void draw();
	static void initializeGPU();
};