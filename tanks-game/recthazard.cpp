#pragma once
#include "recthazard.h"
#include "renderer.h"

VertexArray* RectHazard::va;
VertexBuffer* RectHazard::vb;
IndexBuffer* RectHazard::ib;

void RectHazard::initializeGPU() {
	float positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	va = new VertexArray();
	vb = new VertexBuffer(positions, 4*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(indices, 6);
}