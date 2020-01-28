#pragma once
#include "bullet.h"
#include "constants.h"
#include "mylib.h"
#include <math.h>
#include "circle.h"
#include "colormixer.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

const double Bullet::default_radius = 4;
Bullet::Bullet(double x_, double y_, double r_, double a, double vel, double acc, char id_) { //TODO: make private?
	this->x = x_;
	this->y = y_;
	this->r = r_;
	this->angle = a;
	this->velocity = vel;
	this->acceleration = acc;
	this->id = id_;
	this->alpha = 100;
}

Bullet::Bullet(double x_, double y_, double r_, double a, double vel, double acc, char id_, std::vector<BulletPower*> bp) : Bullet(x_,y_,r_,a,vel,acc,id_) {
	bulletPowers = bp;

	for (int i = 0; i < bulletPowers.size(); i++) {
		bulletPowers[i]->initialize(this);
	}
}

bool Bullet::isDead() {
	return (alpha <= 0);
}

VertexArray* Bullet::va;
VertexBuffer* Bullet::vb;
IndexBuffer* Bullet::ib;

void Bullet::initializeGPU() {
	float positions[(Circle::numOfSides+1)*2];
	for (int i = 0; i < Circle::numOfSides; i++) {
		positions[i*2]   = cos(i * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin(i * 2*PI / Circle::numOfSides);
	}
	positions[Circle::numOfSides*2]   = 0;
	positions[Circle::numOfSides*2+1] = 0;

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = Circle::numOfSides;
		indices[i*3+1] = i;
		indices[i*3+2] = (i+1) % Circle::numOfSides;
	}

	va = new VertexArray();
	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va->AddBuffer(*vb, layout);

	ib = new IndexBuffer(indices, Circle::numOfSides*3);
}

double Bullet::getAngle() {
	return fmod(fmod(angle, 2*PI) + 2*PI, 2*PI);
}

void Bullet::move() {
	bool modifiedMovement = false;
	bool overridedMovement = false;
	bool noMoreMovement = false;

	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->modifiesMovement) {
			if (bulletPowers[i]->modifiedMovementCanOnlyWorkIndividually && modifiedMovement) {
				continue;
			}
			if (noMoreMovement) {
				continue;
			}
			modifiedMovement = true;
			if (bulletPowers[i]->overridesMovement) {
				overridedMovement = true;
			}
			if (!bulletPowers[i]->modifiedMovementCanWorkWithOthers) {
				noMoreMovement = true;
			}
			bulletPowers[i]->modifiedMovement(this);
		}
	}

	if (!overridedMovement) {
		velocity += acceleration;
		x += velocity * cos(angle);
		y += velocity * sin(angle);
	}
}

void Bullet::powerCalculate() {
	for (int i = bulletPowers.size() - 1; i >= 0; i--) {
		bulletPowers[i]->tick(); //I don't think any power will use this, but whatever
		if (bulletPowers[i]->isDone()) {
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			bulletPowers[i]->powerTick(this);
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

void Bullet::drawCPU() {
	drawCPU(x, y);
}

void Bullet::drawCPU(double xpos, double ypos) {
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

void Bullet::draw() {
	draw(x, y);
}

void Bullet::draw(double xpos, double ypos) {
	drawBody(xpos, ypos);
	drawOutline(xpos, ypos);
}

void Bullet::drawBody(double xpos, double ypos) {
	ColorValueHolder color = getColor();

	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);

	if (glIsEnabled(GL_BLEND)) {
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), this->alpha/100);
	} else {
		if(alpha < 100) {
			shader->setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
			glm::mat4 MVPM_2 = Renderer::GenerateMatrix((r+2) * 9/8.0, (r+2) * 9/8.0, PI/2, xpos, ypos);
			double deathPercent = constrain<double>(alpha/100, 0, 1);
			unsigned int deathVertices = Circle::numOfSides * deathPercent;
			shader->setUniformMat4f("u_MVP", MVPM_2);
			Renderer::Draw(*va, *ib, *shader, deathVertices*3);
		}

		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), this->alpha / 100);
	}

	shader->setUniformMat4f("u_MVP", MVPM);
	Renderer::Draw(*va, *ib, *shader);
}

void Bullet::drawOutline(double xpos, double ypos) {
	Shader* shader = Renderer::getShader("main");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f); //lines still look ugly even with glEnable(GL_LINE_SMOOTH), so I don't know what to set it at
	
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos); //duplication but necessary for body/outline methods to be separate
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(GL_LINE_LOOP, 0, Circle::numOfSides);
	
	//cleanup:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//drawing an outline: use a geometry shader (ugh) or another VAO+IBO (lesser ugh), the CPU (big ugh), or glDrawArrays with GL_LINE_LOOP (yay!)
}

short Bullet::determineDamage() { //TODO: finish once powers start existing
	return 1;
}

double Bullet::getHighestOffenseImportance() {
	double highest = -1; //anything below -1 is really, really unimportant; so much so that it doesn't matter
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() > highest) {
			highest = bulletPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

double Bullet::getHighestOffenseTier(double importance) {
	double highest = -999; //TODO: define these constants somewhere
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() == importance) {
			if (bulletPowers[i]->getOffenseTier(this) > highest) {
				highest = bulletPowers[i]->getOffenseTier(this);
			}
		}
	}
	if (bulletPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Bullet::getOffenseTier() {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double Bullet::getHighestDefenseImportance() {
	double highest = -1; //anything below -1 is really, really unimportant; so much so that it doesn't matter
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() > highest) {
			highest = bulletPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

double Bullet::getHighestDefenseTier(double importance) {
	double highest = -999; //TODO: define these constants somewhere
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() == importance) {
			if (bulletPowers[i]->getDefenseTier(this) > highest) {
				highest = bulletPowers[i]->getDefenseTier(this);
			}
		}
	}
	if (bulletPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Bullet::getDefenseTier() {
	return getHighestDefenseTier(getHighestDefenseImportance());
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