#pragma once
#include "constants.h"
#include "bullet.h"
#include <math.h>
#include "circle.h"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char id_) {
	this->x = x_;
	this->y = y_;
	this->r = r_;
	this->angle = a;
	this->velocity = vel;
	this->id = id_;
}

void Bullet::move() {
	x += velocity*cos(angle);
	y += velocity*sin(angle);
}

void Bullet::draw() {
	draw(x, y);
}

void Bullet::draw(double xpos, double ypos) {
	//main body:
	glColor3f(defaultColor.getRf(), defaultColor.getGf(), defaultColor.getBf());

	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex2f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides));
	}

	glEnd();
}

short Bullet::determineDamage() { //TODO: finish once powers start existing
	return 1;
}

bool Bullet::isPartiallyOutOfBounds() {
	return ((x + r > GAME_WIDTH) || (x - r < 0) || (y + r > GAME_HEIGHT) || (y - r < 0));
} //care about equals edge?

bool Bullet::isFullyOutOfBounds() {
	return ((x - r >= GAME_WIDTH) || (x + r <= 0) || (y - r >= GAME_HEIGHT) || (y + r <= 0));
}

Bullet::~Bullet() {
	//remove powers
	return;
}