#pragma once
#include "wall.h"
#include "constants.h"
#include "renderer.h"

#include "GL/glew.h"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
	color = c;
}
Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c, short id_) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
	color = c;
	this->id = id_;
}

void Wall::draw() {
	float positions[] = {
		x, y,
		x + w, y,
		x + w, y + h,
		x, y + h
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va;
	VertexBuffer vb(positions, 4*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	Shader shader = Shader("res/shaders/uniform-vertex.shader", "res/shaders/uniform-fragment.shader");
	shader.Bind();
	shader.setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader.setUniformMat4f("u_MVPM", proj);

	Renderer::Draw(va, ib, shader);
}

void Wall::CPUdraw() {
	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_POLYGON);

	glVertex3f(x, y, 0);
	glVertex3f(x + w, y, 0);
	glVertex3f(x + w, y + h, 0);
	glVertex3f(x, y + h, 0);

	glEnd();
}