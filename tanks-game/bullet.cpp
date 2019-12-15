#pragma once
#include "bullet.h"
#include "constants.h"
#include <math.h>
#include "circle.h"
#include "colormixer.h"

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

Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char id_, std::vector<BulletPower*> bp) : Bullet(x_,y_,r_,a,vel,id_) {
	bulletPowers = bp;
}

void Bullet::move() {
	x += velocity*cos(angle);
	y += velocity*sin(angle);
}

void Bullet::powerCalculate() {
	for (int i = bulletPowers.size() - 1; i >= 0; i--) {
		bulletPowers[i]->tick(); //I don't think any power will use this, but whatever
		if (bulletPowers[i]->isDone()) {
			removePower(i);
		}
		else { //to make each power last its full length, not n-1 length
			bulletPowers[i]->powerTick();
		}
	}
}

void Bullet::removePower(int i) {
	bulletPowers[i]->removeEffects(this);
	delete bulletPowers[i];
	bulletPowers.erase(bulletPowers.begin() + i);
}

ColorValueHolder Bullet::getColor() {
	if (bulletPowers.size() == 0) {
		return defaultColor;
	} else {
		return ColorMixer::mix(bulletPowers);
	}
}

void Bullet::draw() {
	draw(x, y);
}

void Bullet::draw(double xpos, double ypos) {
	//main body:
	ColorValueHolder color = getColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(1);

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
	for (int i = 0; i < bulletPowers.size(); i++) {
		delete bulletPowers[i];
	}
	bulletPowers.clear();
}