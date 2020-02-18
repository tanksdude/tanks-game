#pragma once
#include "hazard.h"
#include "rect.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectHazard : public Hazard, public Rect {
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
public:
	static void initializeGPU();
};