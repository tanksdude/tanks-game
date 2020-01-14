#pragma once
#include "wall.h"
#include "constants.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c, short id_) : Wall(x_, y_, w_, h_, c){
	this->id = id_;
}

VertexArray* Wall::va;
VertexBuffer* Wall::vb;
IndexBuffer* Wall::ib;

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

	va = new VertexArray();
	vb = new VertexBuffer(positions, 4*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(indices, 6);
}

void Wall::draw() {
	Shader* shader = Renderer::getShader("scaling");
	//shader->Bind();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	glm::mat4 trans = glm::translate(proj, glm::vec3(x, y, 0.0f));
	glm::mat4 sc = glm::scale(trans, glm::vec3(w, h, 0));
	shader->setUniformMat4f("u_SM", sc);

	Renderer::Draw(*va, *ib, *shader);
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