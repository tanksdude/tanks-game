#include "tank.h"
#include "gamemanager.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "colormixer.h"
#include "renderer.h"
#include "keypressmanager.h"
#include "bulletmanager.h"
#include "collisionhandler.h"
#include <iostream>

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

TankInputChar::TankInputChar(bool special, int c) {
	isSpecial = special;
	character = c;
}
TankInputChar::TankInputChar() {
	isSpecial = false;
	character = '`';
}

bool TankInputChar::getKeyState() {
	if (isSpecial) {
		return KeypressManager::getSpecialKey(character);
	}
	return KeypressManager::getNormalKey(character);
}

VertexArray* Tank::va;
VertexBuffer* Tank::vb;
IndexBuffer* Tank::ib;
VertexArray* Tank::cannon_va;
VertexBuffer* Tank::cannon_vb;
bool Tank::initialized_GPU = 0;

Tank::Tank(double x_, double y_, double a, char id_, std::string name_, TankInputChar forward, TankInputChar left, TankInputChar right, TankInputChar shoot) {
	x = x_;
	y = y_;
	angle = a;
	gameID = GameManager::getNextID();
	teamID = id_;
	r = TANK_RADIUS;
	name = name_;

	shootCount = 0;
	maxShootCount = 100; //will change whenever while I'm testing

	this->forward = forward;
	this->turnL = left;
	this->turnR = right;
	this->shooting = shoot;

	shootingPoints = new std::vector<CannonPoint>;
	determineShootingAngles();

	initializeGPU();
}

Tank::~Tank() {
	//delete explosionColor;
	for (int i = 0; i < tankPowers.size(); i++) {
		delete tankPowers[i];
	}
	tankPowers.clear();

	delete shootingPoints;

	//uninitializeGPU();
}

bool Tank::initializeGPU() {
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

	/*
	for (int i = 0; i < Circle::numOfSides+1; i++) {
		std::cout << i << ": " << positions[i*2] << " " << positions[i*2+1] << std::endl;
	}
	for (int i = 0; i < Circle::numOfSides; i++) {
		std::cout << i << ": " << indices[i*3] << " " << indices[i*3+1] << " " << indices[i*3+2] << std::endl;
	}
	*/

	vb = new VertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), GL_DYNAMIC_DRAW);
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

bool Tank::uninitializeGPU() {
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

void Tank::move() {
	//TODO: go through each power in the power vector, then use the function that it points to for movement; if none modified movement, use this default

	//if (!forward || !document.getElementById("moveturn").checked) { //change && to || and remove second ! to flip playstyle
		if (turnL.getKeyState()) {
			angle += PI / turningIncrement;
		}
		if (turnR.getKeyState()) {
			angle -= PI / turningIncrement;
		}
	//}
	//if (!document.getElementById("acceleration").checked) {
		if (forward.getKeyState())
			velocity += acceleration;
		else
			velocity -= acceleration; //can result in negative velocities, but that's okay, altered in terminalVelocity()
		terminalVelocity();
	//}
	/*else {
		if (forward.getKeyState())
			velocity = maxSpeed;
		else
			velocity = 0;
	}*/

	x += cos(angle) * velocity;
	y += sin(angle) * velocity;
}

void Tank::terminalVelocity() {
	if (velocity > maxSpeed + acceleration) {
		velocity -= acceleration;
		if (forward.getKeyState() && velocity > maxSpeed) //so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could
			velocity -= acceleration;
	}
	else if (velocity > maxSpeed)
		velocity = maxSpeed;
	else if (velocity < 0)
		velocity = 0;
}

void Tank::shoot() {
	//TODO: allow it to handle multiple shooting cooldowns? (not the point of the game (if it was, shooting cooldown color = mix(white, power color)))
	if(shootCount > 0) //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;

	if(shooting.getKeyState() && shootCount <= 0){
		determineShootingAngles();
		bool modifiedAdditionalShooting = false;
		bool overridedShooting = false;
		bool noMoreOtherAdditionalShooting = false;

		for (int j = 0; j < tankPowers.size(); j++) {
			if (tankPowers[j]->modifiesAdditionalShooting) {
				if (tankPowers[j]->additionalShootingCanOnlyWorkIndividually && modifiedAdditionalShooting) {
					continue;
				}
				if (noMoreOtherAdditionalShooting) {
					continue;
				}
				modifiedAdditionalShooting = true;
				if (tankPowers[j]->overridesAdditionalShooting) {
					overridedShooting = true;
				}
				if (!tankPowers[j]->additionalShootingCanWorkWithOthers) {
					noMoreOtherAdditionalShooting = true;
				}
				for (int i = 0; i < shootingPoints->size(); i++) {
					tankPowers[j]->additionalShooting(this, shootingPoints->at(i));
				}
			}
		}

		if (!overridedShooting){
			for (int i = 0; i < shootingPoints->size(); i++) {
				defaultMakeBullet(shootingPoints->at(i).angle + angle);
			}
		}
		//makeBullet(x + r*cos(angle), y + r*sin(angle), angle, r/4, maxSpeed*2, bp); //should be maxSpeed*4 //this is old, don't look at for too long

		shootCount = maxShootCount * getShootingSpeedMultiplier();
	}
}

void Tank::makeBulletCommon(double x, double y, double angle, double radius, double speed) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(tankPowers.size());
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}

	Bullet* temp = new Bullet(x, y, radius, angle, speed, getTeamID(), bp);
	BulletManager::pushBullet(temp);

	delete bp;
}

void Tank::makeBullet(double x, double y, double angle, double radius, double speed, double acc) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(tankPowers.size());
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}

	Bullet* temp = new Bullet(x, y, radius, angle, speed, acc, getTeamID(), bp, true);
	BulletManager::pushBullet(temp);

	delete bp;
	//bp can be deleted here or in the Bullet constructor, since every bullet should get a different bulletpower vector
}

void Tank::defaultMakeBullet(double angle) {
	makeBulletCommon(x + r*cos(angle), y + r*sin(angle), angle, r*BULLET_TO_TANK_RADIUS_RATIO, maxSpeed*BULLET_TO_TANK_SPEED_RATIO);
}

void Tank::regularMakeBullet(double x, double y, double angle) {
	makeBulletCommon(this->x + x, this->y + y, angle, r*BULLET_TO_TANK_RADIUS_RATIO, maxSpeed*BULLET_TO_TANK_SPEED_RATIO);
}

void Tank::determineShootingAngles() {
	shootingPoints->clear();
	shootingPoints->push_back(CannonPoint(0));

	bool modifiedAdditionalShooting = false;
	bool noMoreAdditionalShootingSpecials = false;

	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->addsShootingPoints) {
			if (tankPowers[i]->addShootingPointsCanOnlyWorkIndividually && modifiedAdditionalShooting) {
				continue;
			}
			if (noMoreAdditionalShootingSpecials) {
				continue;
			}

			modifiedAdditionalShooting = true;
			if (!tankPowers[i]->additionalShootingCanWorkWithOthers) {
				noMoreAdditionalShootingSpecials = true;
			}

			tankPowers[i]->addShootingPoints(this, shootingPoints);
		}
	}
}

double Tank::getShootingSpeedMultiplier() {
	//so this function will look at the firing rate multipliers provided by the tankpowers
	//(0-1] range: use lowest; (1-inf) range: use highest
	//if there are values in each range, then there are three options:
	//1. return either lowest or highest; 2. return average of lowest and highest; 3. return lowest * highest
	//1 is dumb, 2 would probably be fine if there aren't *8 or /8 multipliers, 3 will probably be what I'll use (they will typically be recipricols, so it makes sense)

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList; //only now have I realized this isn't a great name; no, it's not a data structure known as a stack

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getFiringRateMultiplier();
		if (tankPowers[i]->firingRateStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
		//technically don't need to check value against 1, now do I?
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	return highest * lowest * value; //unintentionally works out cleanly
}

void Tank::updateAllValues() {
	updateMaxSpeed();
	updateAcceleration();
	updateRadius();
}

/*
// C++ really sucks sometimes
// this was going to make the copy & pasted code so much cleaner, but C++ gotta be like C sometimes
typedef double (TankPower::*memberFuncPointer)(void);
void Tank::updateSpecificValue(double& attribute, double (TankPower::*func)(void), bool (TankPower::*b), double multiplier) {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	memberFuncPointer p = &TankPower::getTankMaxSpeedMultiplier;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = func();
		if (tankPowers[i]->tankMaxSpeedStacks) {
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

	attribute = highest * lowest * value * multiplier;
}
*/

void Tank::updateMaxSpeed() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankMaxSpeedMultiplier();
		if (tankPowers[i]->tankMaxSpeedStacks) {
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

	maxSpeed = highest * lowest * value;
}

void Tank::updateAcceleration() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankAccelerationMultiplier();
		if (tankPowers[i]->tankAccelerationStacks) {
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

	acceleration = highest * lowest * value * 1.0/16;
}

void Tank::updateRadius() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankRadiusMultiplier();
		if (tankPowers[i]->tankRadiusStacks) {
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

	r = highest * lowest * value * TANK_RADIUS;
}

void Tank::powerCalculate() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		tankPowers[i]->tick(this); //I don't think any power will use this, but whatever
		if (tankPowers[i]->isDone()) {
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			tankPowers[i]->powerTick(this);
		}
	}
}

void Tank::removePower(int index) {
	tankPowers[index]->removeEffects(this);
	delete tankPowers[index];
	tankPowers.erase(tankPowers.begin() + index);
	determineShootingAngles();
	updateAllValues();
}

void Tank::powerReset() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		removePower(i);
	}
}

ColorValueHolder Tank::getBodyColor() {
	if (tankPowers.size() == 0) {
		return defaultColor;
	} else {
		return ColorMixer::mix(&tankPowers);
	}
}

double Tank::getAngle() {
	return fmod(fmod(angle, 2*PI) + 2*PI, 2*PI);
}

double Tank::getCannonAngle(int i) {
	return fmod(fmod(shootingPoints->at(i).angle, 2*PI) + 2*PI, 2*PI);
}

double Tank::getRealCannonAngle(int i) {
	return fmod(fmod(shootingPoints->at(i).angle + angle, 2*PI) + 2*PI, 2*PI);
}

void Tank::drawCPU() {
	//TODO: need ability for more special drawing
	drawCPU(x, y);
}

void Tank::drawCPU(double xpos, double ypos) {

	//shooting cooldown outline:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glVertex3f(xpos, ypos, 0);
	for (int i = 0; i < Circle::numOfSides, (double)i / Circle::numOfSides < shootCount/(maxShootCount*getShootingSpeedMultiplier()); i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides + angle) * 5/4, ypos + r*sin(i * 2*PI / Circle::numOfSides + angle) * 5/4, 0);
	}
	glVertex3f(xpos, ypos, 0);

	glEnd();

	//power cooldown outlines:
	//first, sort by timeLeft/maxTime
	std::vector<TankPower*> sortedTankPowers; //there shouldn't be more than a few powers, so no need to do anything more complex than an array
	sortedTankPowers.reserve(tankPowers.size());
	for (int i = 0; i < tankPowers.size(); i++) {
		//insertion sort because I don't want to think about something more complex for something this small
		//insertion sort has best case O(n) when the list is mostly/entirely sorted, which is possible to get but I don't because it's reversed (easy fix, do later)
		sortedTankPowers.push_back(tankPowers[i]);
		for (int j = sortedTankPowers.size() - 1; j >= 1; j--) {
			if (sortedTankPowers[j]->timeLeft/sortedTankPowers[j]->maxTime > sortedTankPowers[j-1]->timeLeft/sortedTankPowers[j-1]->maxTime){
				std::swap(sortedTankPowers[j], sortedTankPowers[j-1]);
			} else {
				break;
			}
		}
	}
	//second, actually draw them
	for (int i = 0; i < sortedTankPowers.size(); i++) {
		ColorValueHolder c = sortedTankPowers[i]->getColor();

		glColor3f(c.getRf(), c.getGf(), c.getBf());
		glBegin(GL_POLYGON);

		glVertex3f(xpos, ypos, 0);
		for (int j = 0; j < Circle::numOfSides, (double)j / Circle::numOfSides < sortedTankPowers[i]->timeLeft/sortedTankPowers[i]->maxTime; j++) {
			glVertex3f(xpos + r*cos(j * 2*PI / Circle::numOfSides + angle) * 9/8, ypos + r*sin(j * 2*PI / Circle::numOfSides + angle) * 9/8, 0);
		}
		glVertex3f(xpos, ypos, 0);

		glEnd();
	}

	//main body:
	ColorValueHolder color = getBodyColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());
	
	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//other barrels:
	glColor4f(.5, .5, .5, .25);
	glLineWidth(2);

	glBegin(GL_LINES);

	for (int i = 1; i < shootingPoints->size(); i++) {
		glVertex2f(x, y);
		glVertex2f(x + r*cos(angle + shootingPoints->at(i).angle), y + r*sin(angle + shootingPoints->at(i).angle));
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

	//barrel:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(x + r*cos(angle), y + r*sin(angle));

	glEnd();
}

void Tank::draw() {
	draw(x, y);
}

void Tank::draw(double xpos, double ypos) {
	//stuff that will be used:
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	//shooting cooldown outline:
	double shootingOutlinePercent;
	if (maxShootCount*getShootingSpeedMultiplier() <= 0 || maxShootCount <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = constrain<double>(shootCount/(maxShootCount*getShootingSpeedMultiplier()), 0, 1);
	}
	unsigned int shootingOutlineVertices = Circle::numOfSides * shootingOutlinePercent;

	if(shootingOutlineVertices > 0) {
		glm::mat4 MVPM_shootingOutline = Renderer::GenerateMatrix(r * 5.0/4.0, r * 5.0/4.0, getAngle(), xpos, ypos);
		
		shader->setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
		shader->setUniformMat4f("u_MVP", MVPM_shootingOutline);

		Renderer::Draw(*va, *ib, *shader, shootingOutlineVertices*3);
	}
	
	//power cooldown outlines:
	//first, sort by timeLeft/maxTime
	std::vector<TankPower*> sortedTankPowers; //there shouldn't be more than a few powers, so no need to do anything more complex than an array
	sortedTankPowers.reserve(tankPowers.size());
	for (int i = 0; i < tankPowers.size(); i++) {
		//insertion sort because I don't want to think about something more complex for something this small
		//insertion sort has best case O(n) when the list is mostly/entirely sorted, which is possible to obtain but that doesn't happen because it's reversed (easy fix, do later)
		sortedTankPowers.push_back(tankPowers[i]);
		for (int j = sortedTankPowers.size() - 1; j >= 1; j--) {
			if (sortedTankPowers[j]->timeLeft/sortedTankPowers[j]->maxTime > sortedTankPowers[j-1]->timeLeft/sortedTankPowers[j-1]->maxTime){
				std::swap(sortedTankPowers[j], sortedTankPowers[j-1]);
			} else {
				break;
			}
		}
	}
	//second, actually draw them
	for (int i = 0; i < sortedTankPowers.size(); i++) {
		double powerOutlinePercent;
		if (sortedTankPowers[i]->maxTime <= 0) {
			powerOutlinePercent = 0;
		} else {
			powerOutlinePercent = constrain<double>(sortedTankPowers[i]->timeLeft/sortedTankPowers[i]->maxTime, 0, 1);
		}
		unsigned int powerOutlineVertices = Circle::numOfSides * powerOutlinePercent;

		if (powerOutlineVertices > 0) {
			glm::mat4 MVPM_powerOutline = Renderer::GenerateMatrix(r * 9.0/8.0, r * 9.0/8.0, getAngle(), xpos, ypos);

			ColorValueHolder c = sortedTankPowers[i]->getColor();
			shader->setUniform4f("u_color", c.getRf(), c.getGf(), c.getBf(), c.getAf());
			shader->setUniformMat4f("u_MVP", MVPM_powerOutline);

			Renderer::Draw(*va, *ib, *shader, powerOutlineVertices*3);
		}
	}

	//main body:
	MVPM = Renderer::GenerateMatrix(r, r, getAngle(), xpos, ypos);

	ColorValueHolder color = getBodyColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);

	//other barrels:
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	//shader->setUniform4f("u_color", .5f, .5f, .5f, .25f); //CPU color
	shader->setUniform4f("u_color", .75f, .75f, .75f, 1.0f);
	
	for (int i = 1; i < shootingPoints->size(); i++) {
		MVPM = Renderer::GenerateMatrix(r, 1, getRealCannonAngle(i), xpos, ypos);
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);
	}

	//outline:
	MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//barrel:
	glLineWidth(2.0f);
	shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);
	MVPM = Renderer::GenerateMatrix(r, 1, getAngle(), xpos, ypos);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);
	
	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Tank::drawNameCPU() {
	drawNameCPU(x, y);
}

void Tank::drawNameCPU(double xpos, double ypos) {
	//this cannot be done on the GPU easily; will find a library for it eventually

	/*
	if (name.size() == 0)
		return;

	const double scaleFactor = 6;

	glPushMatrix();

	double totalWidth = 0;
	double* widths = new double[name.size()];
	double fillWidth;
	for (int i = 0; i < name.size(); i++) {
		totalWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, name[i]);
		widths[i] = glutStrokeWidth(GLUT_STROKE_ROMAN, name[i]);
	}
	fillWidth = totalWidth/scaleFactor;

	double height = glutStrokeHeight(GLUT_STROKE_ROMAN);

	double center;
	if(xpos - fillWidth/2 < 0){
		center = fillWidth/2;
	}else if(xpos + fillWidth/2 > GAME_WIDTH){
		center = GAME_WIDTH - fillWidth/2;
	}else{
		center = xpos;
	}

	//TODO: add ability for drawName to just draw the name, no adjustment based on y-pos (probably just need a boolean in the parameters)
	double centerY;
	if(ypos + r + 8 + height/scaleFactor > GAME_HEIGHT){
		centerY = ypos - r - 8 - height/scaleFactor;
	} else {
		centerY = ypos + r + 8;
	}
	glTranslatef(center - fillWidth/2, centerY, 0);
	glScalef(1.0/scaleFactor, 1.0/scaleFactor, 1.0/scaleFactor);
	for (int i = 0; i < name.size(); i++) {
		glColor3f(defaultNameStroke.getRf(), defaultNameStroke.getGf(), defaultNameStroke.getBf());
		glLineWidth(8);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, name[i]);

		glTranslatef(-widths[i], 0, 0);

		glColor3f(defaultNameFill.getRf(), defaultNameFill.getGf(), defaultNameFill.getBf());
		glLineWidth(2);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, name[i]);
	}

	delete[] widths;

	glPopMatrix();
	*/
}

void Tank::resetThings(double x, double y, double a, char teamID, std::string name) { //TODO: finish?
	this->x = x;
	this->y = y;
	this->angle = a;
	this->gameID = GameManager::getNextID(); //should this be updated?
	this->teamID = teamID;
	//this->r = TANK_RADIUS;
	this->name = name;
	shootCount = 0;
	//don't update maxShootCount
	velocity = 0;

	if (randFunc() < 1.0/4096) {
		//shiny tank (yes, 1/8192 is the chance before Sword/Shield)
		defaultColor = ColorValueHolder(.75f, .75f, .75f);
	} else {
		defaultColor = ColorValueHolder(.5f, .5f, .5f);
	}

	this->powerReset();
	determineShootingAngles();
}

void Tank::edgeConstrain() {
	CollisionHandler::edgeConstrain(this);
}

double Tank::getHighestOffenseImportance() {
	double highest = -1; //anything below -1 is really, really unimportant; so much so that it doesn't matter
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getOffenseImportance() > highest) {
			highest = tankPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

double Tank::getHighestOffenseTier(double importance) {
	double highest = -999; //TODO: define these constants somewhere or just have a bool for initialization
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getOffenseImportance() == importance) {
			if (tankPowers[i]->getOffenseTier(this) > highest) {
				highest = tankPowers[i]->getOffenseTier(this);
			}
		}
	}
	if (tankPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Tank::getOffenseTier() {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double Tank::getHighestDefenseImportance() {
	double highest = -1; //anything below -1 is really, really unimportant; so much so that it doesn't matter
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getDefenseImportance() > highest) {
			highest = tankPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

double Tank::getHighestDefenseTier(double importance) {
	double highest = -999; //TODO: define these constants somewhere or just have a bool for initialization
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getDefenseImportance() == importance) {
			if (tankPowers[i]->getDefenseTier(this) > highest) {
				highest = tankPowers[i]->getDefenseTier(this);
			}
		}
	}
	if (tankPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Tank::getDefenseTier() {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

bool Tank::isPartiallyOutOfBounds() {
	return CollisionHandler::partiallyOutOfBoundsIgnoreEdge(this);
} //doesn't care if touching edge

bool Tank::isFullyOutOfBounds() {
	return CollisionHandler::fullyOutOfBoundsIgnoreEdge(this);
}
