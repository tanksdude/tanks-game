#pragma once

#include "backgroundrect.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include "constants.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

ColorValueHolder BackgroundRect::backColor = ColorValueHolder(0xDD/255.0, 0xDD/255.0, 0xDD/255.0);
ColorValueHolder BackgroundRect::getBackColor() {
	return backColor;
}

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

	//va = new VertexArray();
	vb = new VertexBuffer(background_positions, 4*2 * sizeof(float), GL_STATIC_DRAW);

	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(background_indices, 6);
}

void BackgroundRect::uninitializeGPU() {
	delete vb;
	delete va;
	delete ib;
}

void BackgroundRect::drawCPU() {
	glColor3f(backColor.getRf(), backColor.getGf(), backColor.getBf());
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(GAME_WIDTH, 0, 0);
	glVertex3f(GAME_WIDTH, GAME_HEIGHT, 0);
	glVertex3f(0, GAME_HEIGHT, 0);
	glEnd();
}

void BackgroundRect::draw() {
	Shader* shader = Renderer::getShader("main");
	shader->setUniform4f("u_color", backColor.getRf(), backColor.getGf(), backColor.getBf(), backColor.getAf());
	//glm::mat4 MVPM = Renderer::GenerateMatrix(GAME_WIDTH, GAME_HEIGHT, 0, 0, 0);
	shader->setUniformMat4f("u_MVP", Renderer::proj);

	Renderer::Draw(*va, *ib, *shader);
}