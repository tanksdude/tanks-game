#pragma once

#include "backgroundrect.h"
#include "renderer.h"
#include "constants.h"

VertexArray* BackgroundRect::va;
VertexBuffer* BackgroundRect::vb;
IndexBuffer* BackgroundRect::ib;

void BackgroundRect::initializeGPU() {
	float background_positions[] = {
		0, 0,
		GAME_WIDTH, 0,
		GAME_WIDTH, GAME_HEIGHT,
		0, GAME_HEIGHT
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	va = new VertexArray();
	vb = new VertexBuffer(background_positions, 4*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(background_indices, 6);
}

void BackgroundRect::draw() {
	Shader* shader = Renderer::getShader("uniform");
	//shader->Bind();
	shader->setUniform4f("u_color", backColor.getRf(), backColor.getGf(), backColor.getBf(), backColor.getAf());
	shader->setUniformMat4f("u_MVPM", proj);

	Renderer::Draw(*va, *ib, *shader);
}