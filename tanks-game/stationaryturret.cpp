#pragma once
#include "stationaryturret.h"
#include "gamemanager.h"
#include "renderer.h"
#include "colormixer.h"
#include "constants.h"
#include <math.h>
#include "tank.h"
#include "bulletmanager.h"
#include "wallmanager.h"
#include "collisionhandler.h"

VertexArray* StationaryTurret::va;
VertexBuffer* StationaryTurret::vb;
IndexBuffer* StationaryTurret::ib;
VertexArray* StationaryTurret::cannon_va;
VertexBuffer* StationaryTurret::cannon_vb;
bool StationaryTurret::initialized_GPU = false;

StationaryTurret::StationaryTurret(double xpos, double ypos, double angle) {
	x = xpos;
	y = ypos;
	this->angle = angle;
	r = TANK_RADIUS / 4;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	tickCycle = 100; //100 is JS default (because of shooting speed) and 200 just looks weird
	maxState = 3;
	stateMultiplier = new double[maxState] {2, 1, 2};
	stateColors = new ColorValueHolder[maxState] { {.5f, .5f, .5f}, {1.0f, 0x22/255.0, 0x11/255.0}, {0, 0.5f, 1.0f} };

	canAcceptPowers = false;

	initializeGPU();
}

StationaryTurret::StationaryTurret(double xpos, double ypos, double angle, double radius) : StationaryTurret(xpos, ypos, angle) {
	r = radius;
}

StationaryTurret::~StationaryTurret() {
	delete[] stateMultiplier;
	delete[] stateColors;

	//uninitializeGPU();
}

bool StationaryTurret::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[(Circle::numOfSides+1)*2];
	positions[0] = 0;
	positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), GL_STATIC_DRAW);
	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(indices, Circle::numOfSides*3);

	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = new VertexBuffer(cannon_positions, 2*2 * sizeof(float));
	VertexBufferLayout cannon_layout(2);
	cannon_va = new VertexArray(*cannon_vb, cannon_layout);

	initialized_GPU = true;
	return true;
}

bool StationaryTurret::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete cannon_va;
	delete cannon_vb;

	initialized_GPU = false;
	return true;
}

CircleHazard* StationaryTurret::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double a = std::stod(argv[2]);
		if (argc >= 4) {
			double r = std::stod(argv[3]);
			return new StationaryTurret(x, y, a, r);
		}
		return new StationaryTurret(x, y, a);
	}
	return new StationaryTurret(0, 0, 0);
}

void StationaryTurret::tick() {
	tickCount++;
	bool mustShoot = false; //in case two state cycles happen at once (this will have annoying unit tests)
	while (tickCount >= tickCycle * stateMultiplier[currentState]) {
		if (tickCycle * stateMultiplier[currentState] <= 0) {
			tickCount = 0;
			currentState = 2;
			mustShoot = true;
			break;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentState];
			++currentState %= maxState;
			if (currentState == 2) {
				mustShoot = true;
			}
		}
	}
	if (mustShoot) {
		BulletManager::pushBullet(new Bullet(x + r*cos(angle), y + r*sin(angle), r/2, angle, 4, 0, -1)); //TODO: make default speed dependent on a constant that Tank uses for its default speed
	}
}

bool StationaryTurret::reasonableLocation() {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}
	return true;
}

ColorValueHolder StationaryTurret::getColor() {
	return ColorMixer::mix(stateColors[currentState], stateColors[(currentState+1)%maxState], tickCount/(tickCycle*stateMultiplier[currentState]));
}

ColorValueHolder StationaryTurret::getColor(short state) {
	if (state < 0) {
		return stateColors[0];
	}
	return stateColors[state % maxState];
}

void StationaryTurret::draw() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, angle, x, y);
	
	//main body:
	ColorValueHolder color = getColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);

	//outline:
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	//shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//barrel:
	glLineWidth(2.0f);
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	MVPM = Renderer::GenerateMatrix(r, 1, angle, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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