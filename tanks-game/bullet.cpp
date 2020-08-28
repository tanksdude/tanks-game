#include "bullet.h"
#include "gamemanager.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include "colormixer.h"
#include "renderer.h"
#include "collisionhandler.h"
#include <iostream>

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

VertexArray* Bullet::va;
VertexBuffer* Bullet::vb;
IndexBuffer* Bullet::ib;
bool Bullet::initialized_GPU = false;

const double Bullet::default_radius = 4;
Bullet::Bullet(double x_, double y_, double a, char teamID, BulletParentType parentType, long parentID) { //every bullet constructor does this stuff
	initializeGPU();
	this->x = x_;
	this->y = y_;
	this->angle = a;
	this->gameID = GameManager::getNextID();
	this->teamID = teamID;
	this->parentType = parentType;
	this->parentID = parentID;
	this->alpha = 100;
}

Bullet::Bullet(double x_, double y_, double a, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp) : Bullet(x_,y_,a,teamID,parentType,parentID) {
	bulletPowers.reserve(bp->size());
	for (int i = 0; i < bp->size(); i++) {
		bulletPowers.push_back(bp->at(i));
	}
	for (int i = 0; i < bulletPowers.size(); i++) {
		bulletPowers[i]->initialize(this);
	}
	//bp not deleted
}

//probably just for banana bullet creation:
Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,a,teamID,parentType,parentID,bp) {
	this->r = r_;
	this->velocity = vel; // * getBulletSpeedMultiplier(); //not wanted
	this->initial_velocity = this->velocity;
	this->acceleration = getBulletAcceleration();
}

//avoid using:
Bullet::Bullet(double x_, double y_, double r_, double a, double vel, double acc, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,a,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity = vel * getBulletSpeedMultiplier();
	this->initial_velocity = this->velocity;
	this->acceleration = acc;
}

//regular 1:
Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char teamID, BulletParentType parentType, long parentID) : Bullet(x_,y_,a,teamID,parentType,parentID) {
	this->r = r_;
	this->velocity = vel;
	this->initial_velocity = vel;
	this->acceleration = 0;
}

//regular 2:
Bullet::Bullet(double x_, double y_, double r_, double a, double vel, char teamID, BulletParentType parentType, long parentID, std::vector<BulletPower*>* bp) : Bullet(x_,y_,a,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity = vel * getBulletSpeedMultiplier();
	this->initial_velocity = this->velocity;
	this->acceleration = getBulletAcceleration();
}

Bullet::~Bullet() {
	for (int i = 0; i < bulletPowers.size(); i++) {
		delete bulletPowers[i];
	}
	bulletPowers.clear();

	//uninitializeGPU();
}

bool Bullet::isDead() const {
	return (alpha <= 0);
}

bool Bullet::initializeGPU() {
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

	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);
	
	ib = new IndexBuffer(indices, Circle::numOfSides*3);

	initialized_GPU = true;
	return true;
}

bool Bullet::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

bool Bullet::canCollideWith(GameThing* thing) const {
	if (this->parentType == BulletParentType::team) {
		return (this->getTeamID() != thing->getTeamID());
	}
	if (this->parentType == BulletParentType::individual) {
		return (parentID != thing->getGameID());
	}
	/*
	if (this->parentType == BulletParentType::name) {
		//TODO
	}
	*/
	return true;
}

bool Bullet::canCollideWith(Bullet* b_other) const {
	if (this->parentType == BulletParentType::individual) {
		return ((parentID != b_other->getGameID()) && (parentID != b_other->getParentID()));
	}
	return canCollideWith((GameThing*)b_other);
}

double Bullet::getAngle() const {
	return fmod(fmod(angle, 2*PI) + 2*PI, 2*PI);
}

void Bullet::move() {
	velocity += acceleration;
	x += velocity * cos(angle);
	y += velocity * sin(angle);
}

double Bullet::getBulletSpeedMultiplier() {
	//look at Tank::getTankFiringRateMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletSpeedMultiplier();
		if (bulletPowers[i]->bulletSpeedStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	return highest * lowest * value;
}

double Bullet::getBulletRadiusMultiplier() {
	//look at Tank::getTankFiringRateMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletRadiusMultiplier();
		if (bulletPowers[i]->bulletRadiusStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}
	return highest * lowest * value;
}

double Bullet::getBulletAcceleration() {
	//look at Tank::getTankFiringRateMultiplier()
	//(this has importance though)

	if (bulletPowers.size() == 0) {
		return 0;
	}

	double importance = -1;
	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletAccelerationImportance();
		if (value > importance) {
			importance = value;
		}
	}

	double highest = 0;
	double lowest = 0;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getBulletAccelerationImportance() == importance) {
			double value = bulletPowers[i]->getBulletAcceleration();
			if (value < 0 && value < lowest) {
				lowest = value;
			} else if (value > 0 && value > highest) {
				highest = value;
			}
		}
	}
	return highest + lowest;
	//return (abs(highest) > abs(lowest) ? highest : lowest);
}

void Bullet::powerCalculate() {
	for (int i = bulletPowers.size() - 1; i >= 0; i--) {
		bulletPowers[i]->tick(this); //I don't think any power will use this, but whatever
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

ColorValueHolder Bullet::getColor() const {
	if (bulletPowers.size() == 0) {
		return defaultColor;
	} else {
		double highest = -1;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() > highest) {
				highest = bulletPowers[i]->getColorImportance();
			}
		}
		std::vector<ColorValueHolder> mixingColors;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(bulletPowers[i]->getColor());
			}
		}
		return ColorMixer::mix(mixingColors.data(), mixingColors.size());
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

void Bullet::draw() const {
	draw(x, y);
}

void Bullet::draw(double xpos, double ypos) const {
	drawBody(xpos, ypos);
	drawOutline(xpos, ypos);
}

void Bullet::poseDraw() const {
	//TODO: just body and outline
	return;
}

void Bullet::drawBody(double xpos, double ypos) const {
	ColorValueHolder color = getColor();
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);

	if (glIsEnabled(GL_BLEND)) {
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), this->alpha/100);
	} else {
		if (alpha < 100) {
			double deathPercent = constrain<double>(alpha/100, 0, 1);
			unsigned int deathVertices = Circle::numOfSides * deathPercent;

			if (deathVertices > 0) {
				glm::mat4 MVPM_deathOutline = Renderer::GenerateMatrix((r+2) * 9.0/8.0, (r+2) * 9.0/8.0, PI/2, xpos, ypos);

				shader->setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
				shader->setUniformMat4f("u_MVP", MVPM_deathOutline);

				Renderer::Draw(*va, *ib, *shader, deathVertices*3);
			}
		}

		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), 1.0f);
	}

	shader->setUniformMat4f("u_MVP", MVPM);
	Renderer::Draw(*va, *ib, *shader);
}

void Bullet::drawOutline(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f); //lines still look ugly even with glEnable(GL_LINE_SMOOTH), so I don't know what to set it at

	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(GL_LINE_LOOP, 1, Circle::numOfSides);
	
	//cleanup:
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//drawing an outline: use a geometry shader (ugh) or another VAO+IBO (lesser ugh), the CPU (big ugh), or glDrawArrays with GL_LINE_LOOP (yay!)
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
	return CollisionHandler::partiallyOutOfBoundsIgnoreEdge(this);
} //care about equals edge?

bool Bullet::isFullyOutOfBounds() {
	return CollisionHandler::fullyOutOfBounds(this);
}
