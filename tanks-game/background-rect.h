#pragma once
#include "color-value-holder.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class BackgroundRect {
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
	//static void drawCPU();

private:
	BackgroundRect() {}
	BackgroundRect(const BackgroundRect&) {}
};
