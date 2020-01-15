#pragma once

#include "backgroundrect.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include "constants.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

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
	//shader->Bind();
	shader->setUniform4f("u_color", backColor.getRf(), backColor.getGf(), backColor.getBf(), backColor.getAf());
	shader->setUniformMat4f("u_MVP", proj); //no need to fully migrate, unless size of board can change...

	Renderer::Draw(*va, *ib, *shader);
}