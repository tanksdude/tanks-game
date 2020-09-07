#include "wall.h"
#include "gamemanager.h"
#include "constants.h"
#include "renderer.h"

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

VertexArray* Wall::va;
VertexBuffer* Wall::vb;
IndexBuffer* Wall::ib;
bool Wall::initialized_GPU = false;

Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
	this->gameID = GameManager::getNextID();
	this->teamID = DEFAULT_TEAM;
	color = c;

	initializeGPU();
}

Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c, char teamID_) : Wall(x_, y_, w_, h_, c) {
	this->teamID = teamID_;
}

Wall::~Wall() {
	//uninitializeGPU();
}

bool Wall::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}
	
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

	vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = new IndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

bool Wall::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

void Wall::draw() const {
	draw(x, y);
}

void Wall::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, xpos, ypos);

	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void Wall::poseDraw() const {
	//TODO: just body, outline, and barrel
	return;
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
