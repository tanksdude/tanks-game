#pragma once
#include "bullet.h"
#include "constants.h"
#include <math.h>
#include "circle.h"
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

Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char id_) { //TODO: make private?
	this->x = x_;
	this->y = y_;
	this->r = r_;
	this->angle = a;
	this->velocity = vel;
	this->id = id_;
}

Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char id_, std::vector<BulletPower*> bp) : Bullet(x_,y_,r_,a,vel,id_) {
	bulletPowers = bp;

	for (int i = 0; i < bulletPowers.size(); i++) {
		bulletPowers[i]->initialize(this);
	}
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
	//main body:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ColorValueHolder color = getColor();

	float positions[(Circle::numOfSides+1)*2];
	for (int i = 0; i < Circle::numOfSides; i++) {
		positions[i*2]   = xpos + r*cos(i * 2*PI / Circle::numOfSides);
		positions[i*2+1] = ypos + r*sin(i * 2*PI / Circle::numOfSides);
	}
	positions[Circle::numOfSides*2]   = xpos;
	positions[Circle::numOfSides*2+1] = ypos;

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = Circle::numOfSides;
		indices[i*3+1] = i;
		indices[i*3+2] = (i+1) % Circle::numOfSides;
	}

	VertexArray va;
	VertexBuffer vb(positions, (Circle::numOfSides+1)*2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push_f(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, Circle::numOfSides*3);

	Shader* shader = Renderer::getShader("uniform");
	//shader->Bind();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVPM", proj);

	Renderer::Draw(va, ib, *shader);

	/*
	//outline:
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1, -1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);

	Renderer::Draw(va, ib, shader);
	
	//cleanup:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
	*/
	//so to draw an outline, either a geometry shader can be used (ugh), the CPU (big ugh), or a fragment shader (lesser ugh)
	//in other words, I'll avoid this for now
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