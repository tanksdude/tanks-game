#pragma once
#include "stationaryturret.h"
#include "renderer.h"
#include "tank.h"

StationaryTurret::StationaryTurret(double xpos, double ypos, double angle) {
	x = xpos;
	y = ypos;
	this->angle = angle;
	r = Tank::default_radius / 4;

	tickCycle = 200;
	maxState = 3;
	stateMultiplier = new short[maxState] {2, 1, 2};
	stateColors = new ColorValueHolder[maxState] {getColor(), {1.0f, 0x22/255.0, 0x11/255.0}, {0, 0.5f, 1.0f} };

	canAcceptPowers = false;
}

StationaryTurret::StationaryTurret(double xpos, double ypos, double angle, double radius) : StationaryTurret(xpos, ypos, angle) {
	r = radius;
}

StationaryTurret::~StationaryTurret() {
	delete[] stateMultiplier;
}

void StationaryTurret::tick() {
	tickCount++;
	bool stateChanged = false;
	if (tickCount >= tickCycle * stateMultiplier[currentState]) {
		tickCount = 0;
		++currentState %= maxState;
		stateChanged = true;
	}

	if (currentState == 0) {

	} else if (currentState == 1) {
		
	} else { //currentState == 2
		if (stateChanged) {
			//fire bullet
		}
	}
}

ColorValueHolder StationaryTurret::getColor() {
	return ColorValueHolder(.5f, .5f, .5f);
}

void StationaryTurret::draw() {
	Shader* shader = Renderer::getShader("main");
	shader->setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	//main body:
	ColorValueHolder color = getColor();

	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	Renderer::Draw(*va, *ib, *shader);

	//outline:
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 0, Circle::numOfSides);

	//cleanup
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void StationaryTurret::drawCPU() {
	//main body:
	ColorValueHolder color = getColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());
	
	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(x + r*cos(i * 2*PI / Circle::numOfSides), y + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex2f(x + r*cos(i * 2*PI / Circle::numOfSides), y + r*sin(i * 2*PI / Circle::numOfSides));
	}

	glEnd();

	//barrel:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(x + r*cos(angle), y + r*sin(angle));

	glEnd();
}