#pragma once

#include "backgroundrect.h"
#include "renderer.h"
#include <glm.hpp>
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
bool BackgroundRect::initialized_GPU = false;

bool BackgroundRect::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

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

	vb = new VertexBuffer(background_positions, 4*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(background_indices, 6);

	initialized_GPU = true;
	return true;
}

bool BackgroundRect::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete vb;
	delete va;
	delete ib;

	initialized_GPU = false;
	return true;
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
	shader->setUniformMat4f("u_MVP", Renderer::getProj());

	Renderer::Draw(*va, *ib, *shader);
}
