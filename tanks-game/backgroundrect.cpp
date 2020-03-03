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
		1, 0,
		1, 1,
		0, 1
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
	glm::mat4 MVPM = Renderer::GenerateMatrix(GAME_WIDTH, GAME_HEIGHT, 0, 0, 0);
	shader->setUniformMat4f("u_MVP", MVPM); //little point in generating a new matrix instead of just using proj but "consistency" and all

	Renderer::Draw(*va, *ib, *shader);
}