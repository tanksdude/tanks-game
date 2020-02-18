#pragma once
#include "hazard.h"
#include "circle.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircleHazard : public Hazard, public Circle {
protected:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
public:
	static void initializeGPU();
	virtual void draw() = 0;
};