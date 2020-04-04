#pragma once
#include "colorvalueholder.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

struct BackgroundRect {
private:
	static ColorValueHolder backColor;
	
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;
public:
	static bool initializeGPU();
	static bool uninitializeGPU();

	static ColorValueHolder getBackColor();

	static void draw();
	static void drawCPU();
};