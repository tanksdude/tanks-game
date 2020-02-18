#pragma once
#include "circlehazard.h"
#include "renderer.h"
#include "constants.h"

VertexArray* CircleHazard::va;
VertexBuffer* CircleHazard::vb;
IndexBuffer* CircleHazard::ib;

void CircleHazard::initializeGPU() {
	float positions[(Circle::numOfSides+1)*2];
	for (int i = 0; i < Circle::numOfSides; i++) {
		positions[i*2]   = cos(i * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin(i * 2*PI / Circle::numOfSides);
	}
	positions[Circle::numOfSides*2]   = 0;
	positions[Circle::numOfSides*2+1] = 0;

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = Circle::numOfSides;
		indices[i*3+1] = i;
		indices[i*3+2] = (i+1) % Circle::numOfSides;
	}

	va = new VertexArray();
	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(indices, Circle::numOfSides*3);
}