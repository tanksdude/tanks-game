#pragma once
#include "tank.h"
#include "constants.h"
#include "colormixer.h"
#include <math.h>
#include <string>
#include <iostream>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

Tank::Tank(double x_, double y_, double a, char id_, std::string name_) {
	x = x_;
	y = y_;
	angle = a;
	id = id_;
	r = 16;
	name = name_;

	shootingPoints = new std::vector<CannonPoint>;
}

Tank::Tank() {
	x = -100;
	y = -100;
	angle = 0;
	id = -1;
	r = 16;
	name = "";

	shootingPoints = new std::vector<CannonPoint>;
}

void Tank::move() {
	//TODO: go through each power in the power vector, then use the function that it points to for movement; if none modified movement, use this default

	//if (!forward || !document.getElementById("moveturn").checked) { //change && to || and remove second ! to flip playstyle
		if (turnL) {
			angle += PI / turningIncrement;
		}
		if (turnR) {
			angle -= PI / turningIncrement;
		}
	//}
	//if (!document.getElementById("acceleration").checked) {
		if (forward)
			velocity += acceleration;
		else
			velocity -= acceleration; //can result in negative velocities, but that's okay, altered in terminalVelocity()
		terminalVelocity();
	//}
	/*else {
		if (forward)
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
		if (forward && velocity > maxSpeed) //so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could
			velocity -= acceleration;
	}
	else if (velocity > maxSpeed)
		velocity = maxSpeed;
	else if (velocity < 0)
		velocity = 0;
}

void Tank::shoot() {
	//TODO: allow it to handle multiple shooting cooldowns(?)
	if(shootCount > 0) //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;

	if(shooting && shootCount <= 0){
		determineShootingAngles();

		for (int i = 0; i < shootingPoints->size(); i++) {
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
					tankPowers[j]->additionalShooting(this, shootingPoints->at(i));
				}
			}

			if (!overridedShooting){
				defaultMakeBullet(x + r*cos(shootingPoints->at(i).angle + angle), y + r*sin(shootingPoints->at(i).angle + angle), shootingPoints->at(i).angle + angle); //should be maxSpeed*4
			}
		}
		//makeBullet(x + r*cos(angle), y + r*sin(angle), angle, r/4, maxSpeed*2, bp); //should be maxSpeed*4

		shootCount = maxShootCount * getShootingSpeedMultiplier();
	}
}

void Tank::makeBullet(double x, double y, double angle, double radius, double speed) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}
	bp->shrink_to_fit();

	Bullet* temp = new Bullet(x, y, radius, angle, speed, id, *bp);
	bullets.push_back(temp);

	//delete bp;
	//don't delete any bp! it's being used! //(doesn't bp need to be deleted though?)
}

inline void Tank::defaultMakeBullet(double x, double y, double angle) {
	makeBullet(x, y, angle, r/4, maxSpeed*2);
}

void Tank::determineShootingAngles() {
	shootingPoints->clear();
	shootingPoints->push_back(CannonPoint(0));
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->addsShootingPoints) {
			tankPowers[i]->addShootingPoints(this, shootingPoints);
		}
	}
}

double Tank::getShootingSpeedMultiplier() {
	//so this function will look at the shooting speed multipliers provided by the tankpowers
	//(0-1] range: use lowest; (1-inf) range: use highest
	//if there are values in each range, then there are three options:
	//1. return either lowest or highest; 2. return average of lowest and highest; 3. return lowest * highest
	//1 is dumb, 2 would probably be fine if there aren't *8 or /8 multipliers, 3 will probably be what I'll use (they will typically be recipricols, so it makes sense)

	double highest = 1;
	double lowest = 1;
	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getShootingMultiplier();
		if (value < 1 && value < lowest) {
			lowest = value;
		} else if (value > 1 && value > highest) {
			highest = value;
		}
		//technically don't need to check value against 1, no do I?
	}
	return highest * lowest; //unintentionally works out cleanly
}

void Tank::powerCalculate() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		tankPowers[i]->tick(); //I don't think any power will use this, but whatever
		if (tankPowers[i]->isDone()) {
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			tankPowers[i]->powerTick();
		}
	}
}

void Tank::removePower(int index) {
	tankPowers[index]->removeEffects(this);
	delete tankPowers[index];
	tankPowers.erase(tankPowers.begin() + index);
	determineShootingAngles();
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
		return ColorMixer::mix(tankPowers);
	}
}

void Tank::draw() {
	//TODO: need ability for more special drawing
	draw(x, y);
}

void Tank::draw(double xpos, double ypos) {

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

void Tank::drawName() {
	drawName(x, y);
}

void Tank::drawName(double xpos, double ypos) {
	//I'm not certain this can be done on the GPU, but I will find out

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

}

void Tank::resetThings(double x, double y, double a, char id, std::string name) { //TODO: finish?
	this->x = x;
	this->y = y;
	this->angle = a;
	this->id = id;
	this->name = name;
	shootCount = 0;
	velocity = 0;

	this->powerReset();
	determineShootingAngles();
}

void Tank::edgeConstrain() {
	if (x + r > GAME_WIDTH) {
		x = GAME_WIDTH - r;
	} else if (x - r < 0) {
		x = r;
	}
	if (y + r > GAME_HEIGHT) {
		y = GAME_HEIGHT - r;
	} else if (y - r < 0) {
		y = r;
	}
	//technically, checking down before up (and left before right) would probably have a slight efficiency increase, but it would be extremely (negligibly) small
}

bool Tank::isPartiallyOutOfBounds() {
	return ((x + r > GAME_WIDTH) || (x - r < 0) || (y + r > GAME_HEIGHT) || (y - r < 0));
} //doesn't care if touching edge

bool Tank::isFullyOutOfBounds() {
	return ((x - r > GAME_WIDTH) || (x + r < 0) || (y - r > GAME_HEIGHT) || (y + r < 0));
}

Tank::~Tank() {
	//delete explosionColor;
	for (int i = 0; i < tankPowers.size(); i++) {
		delete tankPowers[i];
	}
	tankPowers.clear();

	delete shootingPoints;
}
