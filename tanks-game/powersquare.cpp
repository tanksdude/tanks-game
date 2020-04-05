#pragma once
#include "powersquare.h"
#include <string>
#include "backgroundrect.h"
#include "colormixer.h"
#include "renderer.h"
#include <glm.hpp>
#include "powerupmanager.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

VertexArray* PowerSquare::va;
VertexBuffer* PowerSquare::vb;
IndexBuffer* PowerSquare::ib_main;
IndexBuffer* PowerSquare::ib_outline;
bool PowerSquare::initialized_GPU = false;

const double PowerSquare::POWER_WIDTH = 6;
const double PowerSquare::POWER_HEIGHT = 6;
const double PowerSquare::POWER_LINE_WIDTH = 1.0/3.0;
const double PowerSquare::POWER_OUTLINE_MULTIPLIER = 1.5;

PowerSquare::PowerSquare(double x_, double y_) {
	x = x_ - PowerSquare::POWER_WIDTH/2;
	y = y_ - PowerSquare::POWER_HEIGHT/2;
	w = PowerSquare::POWER_WIDTH;
	h = PowerSquare::POWER_HEIGHT;

	initializeGPU();
}

PowerSquare::PowerSquare(double x_, double y_, std::string name) : PowerSquare(x_, y_){
	numOfPowers = 1;
	heldPower = new Power*[1];
	heldPower[0] = PowerupManager::getPowerFactory(name)();
}

PowerSquare::PowerSquare(double x_, double y_, std::string* names, int num) : PowerSquare(x_, y_){
	numOfPowers = num;
	heldPower = new Power*[num];
	for (int i = 0; i < num; i++) {
		heldPower[i] = PowerupManager::getPowerFactory(names[i])();
	}
}

PowerSquare::~PowerSquare() {
	for (int i = 0; i < numOfPowers; i++) {
		delete heldPower[i];
	}
	delete[] heldPower;

	//uninitializeGPU();
}

bool PowerSquare::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float extendingMultiplier = PowerSquare::POWER_LINE_WIDTH * (PowerSquare::POWER_OUTLINE_MULTIPLIER - 1);

	float positions[] = {
		//outer ring (not part of the main stuff)
		0 - extendingMultiplier, 0 - extendingMultiplier, //0
		1 + extendingMultiplier, 0 - extendingMultiplier, //1
		1 + extendingMultiplier, 1 + extendingMultiplier, //2
		0 - extendingMultiplier, 1 + extendingMultiplier, //3

		//main ring
		0, 0, //4
		1, 0, //5
		1, 1, //6
		0, 1, //7

		//inner ring
		0 + PowerSquare::POWER_LINE_WIDTH, 0 + PowerSquare::POWER_LINE_WIDTH, //8
		1 - PowerSquare::POWER_LINE_WIDTH, 0 + PowerSquare::POWER_LINE_WIDTH, //9
		1 - PowerSquare::POWER_LINE_WIDTH, 1 - PowerSquare::POWER_LINE_WIDTH, //10
		0 + PowerSquare::POWER_LINE_WIDTH, 1 - PowerSquare::POWER_LINE_WIDTH  //11
	};
	unsigned int outline_indices[] = { //trapezoids
		//bottom
		0, 1, 5,
		5, 4, 0,

		//right
		1, 2, 6,
		6, 5, 1,

		//left
		4, 7, 3,
		3, 0, 4,

		//top
		2, 3, 7,
		7, 6, 2
	};
	unsigned int main_indices[] = { //trapezoids
		//bottom
		4, 5, 9,
		9, 8, 4,

		//right
		5, 6, 10,
		10, 9, 5,

		//left
		8, 11, 7,
		7, 4, 8,

		//top
		6, 7, 11,
		11, 10, 6
	};

	vb = new VertexBuffer(positions, 12*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib_main = new IndexBuffer(main_indices, 6*4);
	ib_outline = new IndexBuffer(outline_indices, 6*4);

	initialized_GPU = true;
	return true;
}

bool PowerSquare::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib_main;
	delete ib_outline;

	initialized_GPU = false;
	return true;
}

ColorValueHolder PowerSquare::getColor() {
	if (numOfPowers == 1) {
		return heldPower[0]->getColor();
	}
	return ColorMixer::mix(heldPower, numOfPowers);
}

void PowerSquare::givePower(Tank* t) {
	for (int i = 0; i < numOfPowers; i++) {
		t->tankPowers.push_back(heldPower[i]->makeTankPower());

		t->tankPowers[t->tankPowers.size()-1]->initialize(t);
	}
	t->determineShootingAngles();
	t->updateValues();
	//good enough for now
}

void PowerSquare::givePower(Bullet*) { return; } //don't think about it now, possibly ever; it's weird
//void givePower(Hazard*);

void PowerSquare::draw() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	ColorValueHolder color = getColor();

	if (numOfPowers > 1) { //move to drawUnder()
		ColorValueHolder backgroundMix = ColorMixer::mix(color, BackgroundRect::getBackColor());
		shader->setUniform4f("u_color", backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf());
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*va, *ib_outline, *shader);
	}

	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib_main, *shader);
}

void PowerSquare::drawCPU() {
	ColorValueHolder color = getColor();
	if (numOfPowers > 1) { //move to drawUnder()
		ColorValueHolder backgroundMix = ColorMixer::mix(color, BackgroundRect::getBackColor());
		glColor3f(backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf());

		glBegin(GL_QUADS);

		//so basically make a rectangle that lines up with a rectangle of the regular PowerSquare but make it extend POWER_OUTLINE_MULTIPLIER times outward

		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0); //bottom
		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH, 0);
		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH, 0);

		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0); //left
		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);

		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0); //top
		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH, 0);
		glVertex3f(x + w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH, 0);

		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0); //right
		glVertex3f(x + w - w*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w - w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);
		glVertex3f(x + w - w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER), 0);

		glEnd();
	}

	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_QUADS);

	glVertex3f(x, y, 0); //bottom
	glVertex3f(x + w, y, 0);
	glVertex3f(x + w, y + h*POWER_LINE_WIDTH, 0);
	glVertex3f(x, y + h*POWER_LINE_WIDTH, 0);

	glVertex3f(x, y, 0); //left
	glVertex3f(x + w*POWER_LINE_WIDTH, y, 0);
	glVertex3f(x + w*POWER_LINE_WIDTH, y + h, 0);
	glVertex3f(x, y + h, 0);

	glVertex3f(x, y + h, 0); //top
	glVertex3f(x + w, y + h, 0);
	glVertex3f(x + w, y + h - h*POWER_LINE_WIDTH, 0);
	glVertex3f(x, y + h - h*POWER_LINE_WIDTH, 0);

	glVertex3f(x + w, y, 0); //right
	glVertex3f(x + w - w*POWER_LINE_WIDTH, y, 0);
	glVertex3f(x + w - w*POWER_LINE_WIDTH, y + h, 0);
	glVertex3f(x + w, y + h, 0);

	glEnd();
}
