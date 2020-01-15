#pragma once

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

struct BackgroundRect {
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;

public:
	static void draw();
	static void drawCPU();
	static void initializeGPU();
};