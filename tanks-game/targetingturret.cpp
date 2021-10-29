#include "targetingturret.h"
#include "gamemanager.h"
#include "renderer.h"
#include "colormixer.h"
//#include "constants.h"
#include <math.h>
#include <algorithm>
#include "mylib.h"
#include "tank.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"
#include <iostream>

VertexArray* TargetingTurret::va;
VertexBuffer* TargetingTurret::vb;
IndexBuffer* TargetingTurret::ib;
VertexArray* TargetingTurret::cannon_va;
VertexBuffer* TargetingTurret::cannon_vb;
VertexArray* TargetingTurret::reticule_va;
VertexBuffer* TargetingTurret::reticule_vb;
bool TargetingTurret::initialized_GPU = false;

TargetingTurret::TargetingTurret(double xpos, double ypos, double angle, bool) : StationaryTurret(xpos, ypos, angle, true) {
	//x = xpos;
	//y = ypos;
	//this->angle = angle;
	r = TANK_RADIUS / 2;
	//gameID = GameManager::getNextID();
	//teamID = HAZARD_TEAM;

	targeting = false;
	targetingCount = 0;
	trackingID = this->getGameID();
	ColorValueHolder temp[2] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	std::copy(temp, temp+2, reticuleColors);

	canAcceptPowers = false; //... true? probably; hazardpowers were thought up with patrolling turret in mind
}

TargetingTurret::TargetingTurret(double xpos, double ypos, double angle) : TargetingTurret(xpos, ypos, angle, true) {
	initializeGPU();
}

TargetingTurret::TargetingTurret(double xpos, double ypos, double angle, double radius) : TargetingTurret(xpos, ypos, angle) {
	r = radius;
}

TargetingTurret::~TargetingTurret() {
	//delete[] stateMultiplier;
	//delete[] stateColors;

	//uninitializeGPU();
}

bool TargetingTurret::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//body:
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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	//cannon:
	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, 2*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout cannon_layout(2);
	cannon_va = VertexArray::MakeVertexArray(*cannon_vb, cannon_layout);

	//targeting reticule:
	//the circle is the same as the body
	float reticule_positions[16] = {
		 0.75f,  0.0f,   1.25f,  0.0f,  //right
		 0.0f,   0.75f,  0.0f,   1.25f, //up
		-0.75f,  0.0f,  -1.25f,  0.0f,  //left
		 0.0f,  -0.75f,  0.0f,  -1.25f  //down
	};
	reticule_vb = VertexBuffer::MakeVertexBuffer(reticule_positions, 16*2 * sizeof(float));
	VertexBufferLayout reticule_layout(2);
	reticule_va = VertexArray::MakeVertexArray(*reticule_vb, reticule_layout);

	initialized_GPU = true;
	return true;
}

bool TargetingTurret::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete cannon_va;
	delete cannon_vb;
	delete reticule_va;
	delete reticule_vb;

	initialized_GPU = false;
	return true;
}

CircleHazard* TargetingTurret::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double a = std::stod(argv[2]);
		if (argc >= 4) {
			double r = std::stod(argv[3]);
			return new TargetingTurret(x, y, a, r);
		}
		return new TargetingTurret(x, y, a);
	}
	return new TargetingTurret(0, 0, 0);
}

inline void TargetingTurret::updateTrackingPos(const Tank* t, bool pointedAt) {
	if (pointedAt) {
		targetingX = t->x;
		targetingY = t->y;
	} else {
		double dist = sqrt(pow(t->x - x, 2) + pow(t->y - y, 2));
		targetingX = x + dist * cos(direction.getAngle());
		targetingY = y + dist * sin(direction.getAngle());
	}
}

void TargetingTurret::tick() {
	//tickCount is unused... maybe targetingCount should replace it...
	if (currentState == 0) { //either tracking a tank or doing nothing
		//should this be else?
		if (targeting) { //tracking tank
			bool tankIsVisible = false; //singular
			for (int i = 0; i < TankManager::getNumTanks(); i++) {
				//check if 1. tank with gameID of trackingID exists, 2. no walls blocking line of sight to tank
				Tank* t = TankManager::getTank(i); //TODO: go through GameManager because it should have every object and their IDs
				if (t->getGameID() == this->trackingID) { //exists
					if (canSeeTank(t)) { //no obstructions
						tankIsVisible = true;
						//main logic here
						turnTowardsTank(t);
						if (isPointedAt(t)) {
							targetingCount++;
							if (targetingCount >= stateMultiplier[0] * tickCycle) {
								currentState = 1;
								targetingCount = 0;
							}
							updateTrackingPos(t, true);
						} else {
							targetingCount--;
							if (targetingCount < 0) {
								targetingCount = 0;
							}
							updateTrackingPos(t, false);
						}
					}
				}
			}
			if (!tankIsVisible) {
				targeting = false;
			}
		}

		if (!targeting) { //looking for a tank
			targetingCount = 0;

			std::vector<bool> tankVisibility; tankVisibility.reserve(TankManager::getNumTanks()); //not using regular arrays so people (including future me) can actually read this
			std::vector<double> distancesToTank; distancesToTank.reserve(TankManager::getNumTanks()); //TODO: option for angle-based selection (look at homing in PowerFunctionHelper)
			for (int i = 0; i < TankManager::getNumTanks(); i++) {
				Tank* t = TankManager::getTank(i); //TODO: go through GameManager because it should have every object and their IDs
				tankVisibility.push_back(canSeeTank(t));
				if (tankVisibility.at(i)) {
					distancesToTank.push_back(sqrt(pow(x - t->x, 2) + pow(y - t->y, 2)));
				} else {
					distancesToTank.push_back(GAME_WIDTH*2 + GAME_HEIGHT*2);
				}
			}

			double minDist = GAME_WIDTH*2 + GAME_HEIGHT*2;
			std::vector<int> tankIndices; tankIndices.reserve(TankManager::getNumTanks()); //multiple tanks can have same distance
			for (int i = 0; i < TankManager::getNumTanks(); i++) {
				if (distancesToTank[i] == minDist) {
					tankIndices.push_back(i);
				} else if (distancesToTank[i] < minDist) {
					tankIndices.clear();
					tankIndices.push_back(i);
					minDist = distancesToTank[i];
				}
			}

			int indexOfTargetedTank;
			if (tankIndices.size() == 1) {
				indexOfTargetedTank = tankIndices[0];
			} else {
				indexOfTargetedTank = tankIndices[int(RNG::randFunc() * tankIndices.size())];
			}

			if (tankVisibility[indexOfTargetedTank]) {
				Tank* t = TankManager::getTank(indexOfTargetedTank);
				trackingID = t->getGameID();
				targeting = true;
				updateTrackingPos(t, isPointedAt(t));
			}
		}
	}
	if (currentState == 1) {
		targetingCount++;
		if (targetingCount >= stateMultiplier[1] * tickCycle) {
			BulletManager::pushBullet(new Bullet(x + r*cos(direction.getAngle()), y + r*sin(direction.getAngle()), r*BULLET_TO_TANK_RADIUS_RATIO, direction.getAngle(), Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO, this->getTeamID(), BulletParentType::individual, this->getGameID()));
			currentState = 2;
			targetingCount = 0;
			targeting = false; //allows target to change (also controls whether the reticule is drawn)
		}
	}
	if (currentState == 2) {
		targetingCount++;
		if (targetingCount >= stateMultiplier[2] * tickCycle) {
			targetingCount = 0;
			currentState = 0;
		}
	}
	//maxState is 3; not using else in case tickCycle is zero
}

bool TargetingTurret::canSeeTank(const Tank* t) const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::lineRectCollision(x, y, t->x, t->y, wa)) {
			return false;
		}
	}
	return true;
}

void TargetingTurret::turnTowardsTank(const Tank* t) {
	//see PowerFunctionHelper::homingGeneric
	SimpleVector2D distToTank = SimpleVector2D(t->getX() - this->x, t->getY() - this->y);
	float theta = SimpleVector2D::angleBetween(distToTank, SimpleVector2D(direction.getAngle(), 0, true));
	if (abs(theta) < PI/turningIncrement) {
		//too small to adjust angle
	} else {
		//large angle adjustment needed
		if (theta < 0) {
			this->direction.changeAngle(PI/turningIncrement);
		} else {
			this->direction.changeAngle(-PI/turningIncrement);
		}
	}
}

bool TargetingTurret::isPointedAt(const Tank* t) const {
	return (abs(SimpleVector2D::angleBetween(SimpleVector2D(direction.getAngle(), 0, true), SimpleVector2D(t->x - x, t->y - y))) < PI/turningIncrement);
}

bool TargetingTurret::reasonableLocation() const {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		if (canSeeTank(TankManager::getTank(i))) {
			return false;
		}
	}

	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getGameID() != this->getGameID()) {
			if (CollisionHandler::partiallyCollided(this, ch)) {
				return false;
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getRectHazard(i))) {
			return false;
		}
	}

	return validLocation();
}

ColorValueHolder TargetingTurret::getColor() const {
	return ColorMixer::mix(stateColors[currentState], stateColors[(currentState+1)%maxState], constrain<double>(targetingCount/(tickCycle*stateMultiplier[currentState]), 0, 1));
}

ColorValueHolder TargetingTurret::getColor(short state) const {
	if (state < 0) {
		return stateColors[0];
	}
	return stateColors[state % maxState];
}

ColorValueHolder TargetingTurret::getReticuleColor() const {
	if (currentState == 0) {
		return reticuleColors[0];
	}
	if (currentState == 1) {
		return reticuleColors[1];
	}
	return ColorValueHolder(0.0f, 0.0f, 0.0f); //shouldn't be drawn
}

void TargetingTurret::draw() const {
	draw(x, y);
}

void TargetingTurret::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, direction.getAngle(), xpos, ypos);

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
	MVPM = Renderer::GenerateMatrix(r, 1, direction.getAngle(), xpos, ypos);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//reticule:
	if (targeting) {
		//glLineWidth(2.0f);
		color = getReticuleColor();
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
		MVPM = Renderer::GenerateMatrix(2*TANK_RADIUS, 2*TANK_RADIUS, 0, targetingX, targetingY);
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
		Renderer::Draw(*reticule_va, *shader, GL_LINES, 0, 8);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TargetingTurret::poseDraw() const {
	//TODO
	return;
}

void TargetingTurret::drawCPU() const {
	//nah
}

CircleHazard* TargetingTurret::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;
	if (argc >= 1) {
		angle = std::stod(argv[0]);
	} else {
		angle = RNG::randFunc() * 2*PI;
	}
	do {
		xpos = RNG::randFunc2() * (area_width - 2*TANK_RADIUS/2) + (x_start + TANK_RADIUS/2);
		ypos = RNG::randFunc2() * (area_height - 2*TANK_RADIUS/2) + (y_start + TANK_RADIUS/2);
		CircleHazard* testTargetingTurret = new TargetingTurret(xpos, ypos, angle);
		if (testTargetingTurret->reasonableLocation()) {
			randomized = testTargetingTurret;
			break;
		} else {
			delete testTargetingTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
