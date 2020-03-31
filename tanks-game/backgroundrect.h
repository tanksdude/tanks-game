#pragma once
#include "colorvalueholder.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

struct BackgroundRect {
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;

	static ColorValueHolder backColor;
public:
	static ColorValueHolder getBackColor();

	static void draw();
	static void drawCPU();
	static void initializeGPU();
	static void uninitializeGPU();
};