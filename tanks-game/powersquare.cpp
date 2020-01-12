#pragma once
#include <string>
#include <iostream>
#include "powersquare.h"
#include "colormixer.h"
#include "renderer.h"

#include <GL/glew.h>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

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
	//good enough for now
}

void PowerSquare::givePower(Bullet*) { return; } //don't think about it now, possibly ever; it's weird
//void givePower(Hazard*);

PowerSquare::PowerSquare(double x_, double y_, std::string name) {
	x = x_ - POWER_WIDTH/2;
	y = y_ - POWER_HEIGHT/2;
	w = POWER_WIDTH;
	h = POWER_HEIGHT;

	numOfPowers = 1;
	heldPower = new Power*[1];
	heldPower[0] = powerLookup[name]();
}

PowerSquare::PowerSquare(double x_, double y_, std::string* names, int num) {
	x = x_ - POWER_WIDTH/2;
	y = y_ - POWER_HEIGHT/2;
	w = POWER_WIDTH;
	h = POWER_HEIGHT;

	numOfPowers = num;
	heldPower = new Power*[num];
	for (int i = 0; i < num; i++) {
		heldPower[i] = powerLookup[names[i]]();
	}
}

PowerSquare::~PowerSquare() {
	for (int i = 0; i < numOfPowers; i++) {
		delete heldPower[i];
	}
	delete[] heldPower;
}

void PowerSquare::draw() {
	//BIG TODO: reduce code duplication
	ColorValueHolder color = getColor();
	Shader* shader = Renderer::getShader("uniform");
	if (numOfPowers > 1) { //move to drawUnder()
		float extendingMultiplier = POWER_LINE_WIDTH*(1 - POWER_OUTLINE_MULTIPLIER);
		ColorValueHolder backgroundMix = ColorMixer::mix(color, backColor);
		float positions[] = { //a bunch of trapezoids
			//bottom
			x, y, //0
			x + w, y, //1
			x + w - w*extendingMultiplier, y + h*extendingMultiplier, //2
			x + w*extendingMultiplier, y + h*extendingMultiplier, //3

			//right
			//x + w, y, //1
			x + w, y + h, //4
			x + w - w*extendingMultiplier, y + h - h*extendingMultiplier, //5
			//x + w - w*extendingMultiplier, y + h*extendingMultiplier, //2

			//left (remember that the indices on this have to go counterclockwise)
			//x, y, //0
			x, y + h, //6
			x + w*extendingMultiplier, y + h - h*extendingMultiplier, //7
			//x + w*extendingMultiplier, y + h*extendingMultiplier, //3

			//top
			//x + w, y + h, //4
			//x, y + h, //6
			//x + w*extendingMultiplier, y + h - h*extendingMultiplier, //7
			//x + w - w*extendingMultiplier, y + h - h*extendingMultiplier //5
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
			1, 4, 5,
			5, 2, 1,
			3, 7, 6,
			6, 0, 3,
			4, 6, 7,
			7, 5, 4
		};

		VertexArray va;
		VertexBuffer vb(positions, 8*2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push_f(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6*4);

		//shader->Bind();
		shader->setUniform4f("u_color", backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf());
		shader->setUniformMat4f("u_MVPM", proj);

		Renderer::Draw(va, ib, *shader);
	}
	//probable TODO: put all vertices in positions, then make two index buffers (then draw)

	float positions[] = { //a bunch of trapezoids
		//bottom
		x, y, //0
		x + w, y, //1
		x + w - w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH, //2
		x + w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH, //3

		//right
		//x + w, y, //1
		x + w, y + h, //4
		x + w - w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH, //5
		//x + w - w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH, //2

		//left (remember that the indices on this have to go counterclockwise)
		//x, y, //0
		x, y + h, //6
		x + w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH, //7
		//x + w*POWER_LINE_WIDTH, y + h*POWER_LINE_WIDTH, //3

		//top
		//x + w, y + h, //4
		//x, y + h, //6
		//x + w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH, //7
		//x + w - w*POWER_LINE_WIDTH, y + h - h*POWER_LINE_WIDTH //5
	}; //TODO: organize it as the four vertices for collision then the four inner vertices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		1, 4, 5,
		5, 2, 1,
		3, 7, 6,
		6, 0, 3,
		4, 6, 7,
		7, 5, 4
	};

	VertexArray va;
	VertexBuffer vb(positions, 8*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6*4);

	//shader->Bind();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVPM", proj);

	Renderer::Draw(va, ib, *shader);
}

void PowerSquare::drawCPU() {
	ColorValueHolder color = getColor();
	if (numOfPowers > 1) { //move to drawUnder()
		ColorValueHolder backgroundMix = ColorMixer::mix(color, backColor);
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
