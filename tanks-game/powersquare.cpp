#pragma once
#include <string>
#include <iostream>
#include "powersquare.h"
#include "colormixer.h"
#include "speedpower.h"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

void PowerSquare::draw() {
	ColorValueHolder color = getColor();
	if (numOfPowers > 1) { //move to drawUnder()
		ColorValueHolder backgroundMix = ColorMixer::mix(color, backColor);
		glColor3f(backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf());

		glBegin(GL_QUADS);

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
	//good enough for now
	//fix: SpeedPower is in PowerSquare, generates a TankPower and a BulletPower for a tank here
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
	//heldPower[0] = nullptr;
	//heldPower[0] = new SpeedPower();
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
