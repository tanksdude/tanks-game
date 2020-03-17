#pragma once
#include "wall.h"
#include "constants.h"
#include "renderer.h"
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>

Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
	color = c;

	initializeGPU();
}
Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c, short id_) : Wall(x_, y_, w_, h_, c){
	this->id = id_;
}

Wall::~Wall() {
	delete va;
	delete vb;
	delete ib;
}

void Wall::initializeGPU() {
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

	//va = new VertexArray();
	vb = new VertexBuffer(positions, 4*2 * sizeof(float));

	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(indices, 6);
}

void Wall::draw() {
	Shader* shader = Renderer::getShader("main");
	//shader->Bind();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void Wall::drawCPU() {
	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_POLYGON);

	glVertex3f(x, y, 0);
	glVertex3f(x + w, y, 0);
	glVertex3f(x + w, y + h, 0);
	glVertex3f(x, y + h, 0);

	glEnd();
}