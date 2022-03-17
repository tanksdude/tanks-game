#include "tank.h"
#include "game-manager.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "color-mixer.h"
#include "background-rect.h"
#include "renderer.h"
#include "keypress-manager.h"
#include "bullet-manager.h"
#include "rng.h"
#include "level-manager.h"
#include <iostream>

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

TankInputChar::TankInputChar(std::string key) {
	if (KeypressManager::keyIsSpecialFromString(key)) {
		isSpecial = true;
		character = KeypressManager::specialKeyFromString(key);
	} else {
		isSpecial = false;
		character = KeypressManager::normalKeyFromString(key);
	}
}
TankInputChar::TankInputChar(bool special, int c) {
	isSpecial = special;
	character = c;
}
TankInputChar::TankInputChar() {
	isSpecial = false;
	character = '`';
}

bool TankInputChar::getKeyState() const {
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
bool Tank::initialized_GPU = false;

const double Tank::default_maxSpeed = 1;
const double Tank::default_acceleration = 1.0/16;
const double Tank::default_turningIncrement = 64;

Tank::Tank(double x_, double y_, double angle, Team_ID teamID, std::string name_, std::string forward, std::string left, std::string right, std::string shoot, std::string special)
: Tank(x_, y_, angle, teamID, name_, TankInputChar(forward), TankInputChar(left), TankInputChar(right), TankInputChar(shoot), TankInputChar(special)) {}

Tank::Tank(double x_, double y_, double angle, Team_ID teamID, std::string name_, TankInputChar forward, TankInputChar left, TankInputChar right, TankInputChar shoot, TankInputChar special) : GameThing(teamID) {
	x = x_;
	y = y_;
	velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS;
	name = name_;
	this->dead = false;

	maxSpeed = default_maxSpeed;
	acceleration = default_acceleration;
	turningIncrement = default_turningIncrement;

	shootCount = 0;
	maxShootCount = 100; //will change whenever while I'm testing

	this->forward = forward;
	this->turnL = left;
	this->turnR = right;
	this->shooting = shoot;
	this->specialKey = special;

	shootingPoints = std::vector<CannonPoint>();
	determineShootingAngles();
	updateAllValues();

	if (RNG::randFunc() < 1.0/4096) {
		//shiny tank (yes, 1/8192 is the chance before Sword/Shield)
		defaultColor = ColorValueHolder(0.75f, 0.75f, 0.75f);
	} else {
		defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f);
	}

	initializeGPU();
}

Tank::Tank(double x_, double y_, double angle, Team_ID id_, std::string name_, TankInputChar* inputs)
: Tank(x_, y_, angle, id_, name_, inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]) {}

Tank::~Tank() {
	for (int i = 0; i < tankPowers.size(); i++) {
		delete tankPowers[i];
	}
	tankPowers.clear();

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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, 2*2 * sizeof(float));
	VertexBufferLayout cannon_layout(2);
	cannon_va = VertexArray::MakeVertexArray(*cannon_vb, cannon_layout);

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

TankInputChar* Tank::getKeys() const {
	return new TankInputChar[]{ forward, turnL, turnR, shooting, specialKey };
}

bool Tank::move() {
	bool shouldBeKilled = false;
	bool modifiedMovement = false;
	bool overridedMovement = false;
	bool noMoreMovementSpecials = false;
	//TODO: handle killing the tankpowers

	for (int k = 0; k < tankPowers.size(); k++) {
		if (tankPowers[k]->modifiesMovement) {
			if (tankPowers[k]->modifiedMovementCanOnlyWorkIndividually && modifiedMovement) {
				continue;
			}
			if (noMoreMovementSpecials) {
				continue;
			}

			modifiedMovement = true;
			if (tankPowers[k]->overridesMovement) {
				overridedMovement = true;
			}
			if (!tankPowers[k]->modifiedMovementCanWorkWithOthers) {
				noMoreMovementSpecials = true;
			}

			InteractionBoolHolder check_temp = tankPowers[k]->modifiedMovement(this);
			if (check_temp.shouldDie) {
				shouldBeKilled = true;
				overridedMovement = true;
				break;
			}
		}
	}

	if (!overridedMovement) {
		move_base();
	}

	return shouldBeKilled;
}

inline void Tank::move_base() {
	//if (!forward || !document.getElementById("moveturn").checked) { //change && to || and remove second ! to flip playstyle
		if (turnL.getKeyState()) {
			velocity.changeAngle(PI / turningIncrement);
		}
		if (turnR.getKeyState()) {
			velocity.changeAngle(-PI / turningIncrement);
		}
	//}
	//if (!document.getElementById("acceleration").checked) {
		if (forward.getKeyState()) {
			velocity.changeMagnitude(acceleration);
		} else {
			velocity.changeMagnitude(-acceleration); //can result in negative velocities, fixed by SimpleVector2D just not allowing that
		}
		terminalVelocity();
	//}
	/*else {
		if (forward.getKeyState())
			velocity = maxSpeed;
		else
			velocity = 0;
	}*/

	x += velocity.getXComp();
	y += velocity.getYComp();
}

inline void Tank::terminalVelocity() {
	if (velocity.getMagnitude() > maxSpeed + acceleration) {
		velocity.changeMagnitude(-acceleration);
		if (forward.getKeyState() && velocity.getMagnitude() > maxSpeed) {
			//so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could (maybe change?)
			velocity.changeMagnitude(-acceleration);
		}
	} else if (velocity.getMagnitude() > maxSpeed) {
		velocity.setMagnitude(maxSpeed);
	} /*else if (velocity.getMagnitude() < 0) {
		velocity.setMagnitude(0);
	}*/
}

void Tank::shoot() {
	//TODO: allow it to handle multiple shooting cooldowns? (not the point of the game (if it was, shooting cooldown color = mix(white, power color)))
	if (shootCount > 0) { //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;
	}

	if (shooting.getKeyState() && shootCount <= 0) {
		//determineShootingAngles(); //TODO: is this needed?
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
				for (int i = 0; i < shootingPoints.size(); i++) {
					tankPowers[j]->additionalShooting(this, shootingPoints.at(i));
				}
			}
		}

		if (!overridedShooting) {
			for (int i = shootingPoints.size() - 1; i >= 0; i--) {
				defaultMakeBullet(shootingPoints[i].angle + velocity.getAngle());
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

	Bullet* temp = new Bullet(x, y, radius, angle, speed, getTeamID(), BulletParentType::team, getGameID(), bp);
	BulletManager::pushBullet(temp);

	delete bp;
}

void Tank::makeBullet(double x, double y, double angle, double radius, double speed, double acc) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(tankPowers.size());
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}

	Bullet* temp = new Bullet(x, y, radius, angle, speed, acc, getTeamID(), BulletParentType::team, getGameID(), bp, true);
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
	shootingPoints.clear();
	shootingPoints.push_back(CannonPoint(0));

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

			tankPowers[i]->addShootingPoints(this, &shootingPoints);
		}
	}
}

double Tank::getShootingSpeedMultiplier() const {
	//so this function will look at the firing rate multipliers provided by the tankpowers
	//(0-1] range: use lowest; (1-inf) range: use highest
	//if there are values in each range, then there are three options:
	//1. return either lowest or highest; 2. return average of lowest and highest; 3. return lowest * highest
	//1 is dumb, 2 would probably be fine if there aren't *8 or /8 multipliers, 3 will probably be what I'll use (they will typically be recipricols, so it makes sense)

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList; //only now have I realized this isn't a great name; no, it's not a data structure known as a stack

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankFiringRateMultiplier();
		if (tankPowers[i]->tankFiringRateStacks) {
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

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankFiringRateMultiplier();
		}
	}

	return highest * lowest * value * level_amount; //unintentionally works out cleanly
}

void Tank::updateAllValues() {
	updateMaxSpeed();
	updateAcceleration();
	updateRadius();
	updateTurningIncrement();
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

	//[insert level effect value getting here]

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

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankMaxSpeedMultiplier();
		}
	}

	maxSpeed = highest * lowest * value * level_amount * default_maxSpeed;
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

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankAccelerationMultiplier();
		}
	}

	acceleration = highest * lowest * value * level_amount * default_acceleration;
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

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankRadiusMultiplier();
		}
	}

	r = highest * lowest * value * level_amount * TANK_RADIUS;
}

void Tank::updateTurningIncrement() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;
	int negativeCount = 0;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankTurningIncrementMultiplier();
		if (value < 0) {
			negativeCount++;
			value = value * -1;
		}
		if (tankPowers[i]->tankTurningIncrementStacks) {
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

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankTurningIncrementMultiplier();
		}
	}

	turningIncrement = highest * lowest * value * level_amount * default_turningIncrement * (negativeCount%2 == 0 ? 1 : -1);
	velocity.setAngle(round(velocity.getAngle() / (PI / turningIncrement)) * (PI / turningIncrement));
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

ColorValueHolder Tank::getBodyColor() const {
	if (tankPowers.size() == 0) {
		return defaultColor;
	} else {
		double highest = LOW_IMPORTANCE;
		for (int i = 0; i < tankPowers.size(); i++) {
			if (tankPowers[i]->getColorImportance() > highest) {
				highest = tankPowers[i]->getColorImportance();
			}
		}
		std::vector<ColorValueHolder> mixingColors;
		for (int i = 0; i < tankPowers.size(); i++) {
			if (tankPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(tankPowers[i]->getColor());
			}
		}
		return ColorMixer::mix(mixingColors.data(), mixingColors.size());
	}
}

double Tank::getAngle() const {
	return fmod(fmod(velocity.getAngle(), 2*PI) + 2*PI, 2*PI);
}

double Tank::getCannonAngle(int i) const {
	return fmod(fmod(shootingPoints[i].angle, 2*PI) + 2*PI, 2*PI);
}

double Tank::getRealCannonAngle(int i) const {
	return fmod(fmod(shootingPoints[i].angle + velocity.getAngle(), 2*PI) + 2*PI, 2*PI);
}

/*
void Tank::drawCPU() const {
	//TODO: need ability for more special drawing
	drawCPU(x, y);
}

void Tank::drawCPU(double xpos, double ypos) const {
	//shooting cooldown outline:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glVertex3f(xpos, ypos, 0);
	for (int i = 0; i < Circle::numOfSides, (double)i / Circle::numOfSides < shootCount/(maxShootCount*getShootingSpeedMultiplier()); i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides + velocity.getAngle()) * 5/4, ypos + r*sin(i * 2*PI / Circle::numOfSides + velocity.getAngle()) * 5/4, 0);
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
			if (sortedTankPowers[j]->timeLeft/sortedTankPowers[j]->maxTime > sortedTankPowers[j-1]->timeLeft/sortedTankPowers[j-1]->maxTime) {
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
			glVertex3f(xpos + r*cos(j * 2*PI / Circle::numOfSides + velocity.getAngle()) * 9/8, ypos + r*sin(j * 2*PI / Circle::numOfSides + velocity.getAngle()) * 9/8, 0);
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
		glVertex2f(x + r*cos(velocity.getAngle() + shootingPoints->at(i).angle), y + r*sin(velocity.getAngle() + shootingPoints->at(i).angle));
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
	glVertex2f(x + r*cos(velocity.getAngle()), y + r*sin(velocity.getAngle()));

	glEnd();
}
*/

void Tank::draw() const {
	if (this->dead) {
		drawDead();
	} else {
		drawShootingCooldown();
		drawPowerCooldown();
		drawBody();
		drawExtraBarrels();
		drawOutline();
		drawMainBarrel();
	}
}

void Tank::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::draw!" << std::endl;
		case DrawingLayers::normal:
			if (!this->dead) {
				drawShootingCooldown();
				drawPowerCooldown();
				drawBody();
				drawExtraBarrels();
				drawOutline();
				drawMainBarrel();
			}
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			if (this->dead) {
				drawDead();
			}
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Tank::poseDraw() const {
	//does not have to worry about being dead
	drawBody();
	//drawExtraBarrels();
	drawOutline();
	drawMainBarrel();
}

void Tank::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			//drawExtraBarrels();
			drawOutline();
			drawMainBarrel();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//drawDead();
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Tank::ghostDraw(float alpha) const {
	//does not have to worry about being dead
	drawBody(alpha);
	drawExtraBarrels(alpha);
	drawOutline(alpha);
	drawMainBarrel(alpha);
}

void Tank::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody(alpha);
			drawExtraBarrels(alpha);
			drawOutline(alpha);
			drawMainBarrel(alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//drawDead();
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void Tank::drawBody(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color;
	if (this->dead) {
		color = ColorValueHolder(0.0f, 0.0f, 0.0f);
		//shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 0.5f); //alpha isn't interpreted
	} else {
		color = getBodyColor();
	}
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

inline void Tank::drawDead(float alpha) const {
	drawBody(alpha);
	drawOutline(alpha);
}

inline void Tank::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(1.0f);
	Renderer::SetLineWidth(1.0f);

	ColorValueHolder color;
	if (this->dead) {
		color = ColorValueHolder(1.0f, 1.0f, 1.0f);
	} else {
		color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	}
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void Tank::drawShootingCooldown(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	double shootingOutlinePercent;
	if (maxShootCount*getShootingSpeedMultiplier() <= 0 || maxShootCount <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = constrain<double>(shootCount/(maxShootCount*getShootingSpeedMultiplier()), 0, 1);
	}
	unsigned int shootingOutlineVertices = Circle::numOfSides * shootingOutlinePercent;

	if (shootingOutlineVertices > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		MVPM = Renderer::GenerateMatrix(r * 5.0/4.0, r * 5.0/4.0, getAngle(), x, y);
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*va, *ib, *shader, shootingOutlineVertices*3);
	}
}

inline void Tank::drawPowerCooldown(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	//first, sort by timeLeft/maxTime
	std::vector<TankPower*> sortedTankPowers; //there shouldn't be more than a few powers, so no need to do anything more complex than an array
	sortedTankPowers.reserve(tankPowers.size());
	for (int i = 0; i < tankPowers.size(); i++) {
		//insertion sort because I don't want to think about something more complex for something this small
		//insertion sort has best case O(n) when the list is mostly/entirely sorted, which is possible to obtain but that doesn't happen because it's reversed (easy fix, do later)
		sortedTankPowers.push_back(tankPowers[i]);
		for (int j = sortedTankPowers.size() - 1; j >= 1; j--) {
			if (sortedTankPowers[j]->timeLeft/sortedTankPowers[j]->maxTime > sortedTankPowers[j-1]->timeLeft/sortedTankPowers[j-1]->maxTime) {
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
			ColorValueHolder c = sortedTankPowers[i]->getColor();
			c = ColorMixer::mix(BackgroundRect::getBackColor(), c, alpha);
			shader->setUniform4f("u_color", c.getRf(), c.getGf(), c.getBf(), c.getAf());

			MVPM = Renderer::GenerateMatrix(r * 9.0/8.0, r * 9.0/8.0, getAngle(), x, y);
			shader->setUniformMat4f("u_MVP", MVPM);

			Renderer::Draw(*va, *ib, *shader, powerOutlineVertices*3);
		}
	}
}

inline void Tank::drawMainBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	Renderer::SetLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, 1, getAngle(), x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void Tank::drawExtraBarrels(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(1.0f);
	Renderer::SetLineWidth(1.0f);

	//shader->setUniform4f("u_color", 0.5f, 0.5f, 0.5f, 0.25f); //CPU color
	ColorValueHolder color = ColorValueHolder(0.75f, 0.75f, 0.75f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	for (int i = 1; i < shootingPoints.size(); i++) {
		MVPM = Renderer::GenerateMatrix(r, 1, getRealCannonAngle(i), x, y);
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Tank::kill() {
	//for checking whether to save the tank, it's first come first served, then no one else
	//if it can save the tank, stop checking, else continue checking

	bool shouldBeKilled = true;

	for (int i = 0; i < tankPowers.size(); i++) {
		bool killTankPower = false;

		if (tankPowers[i]->modifiesDeathHandling) {
			InteractionBoolHolder check_temp = tankPowers[i]->modifiedDeathHandling(this);
			if (!check_temp.shouldDie) {
				shouldBeKilled = false;
			}
			if (check_temp.otherShouldDie) {
				killTankPower = true;
			}
		}

		if (killTankPower) {
			removePower(i);
			i--;
			//continue;
		}
		if (!shouldBeKilled) {
			break;
		}
	}

	if (shouldBeKilled) {
		this->dead = shouldBeKilled;
	}

	return this->dead;
}

void Tank::kill_hard() {
	this->dead = true;
}

/*
void Tank::resetThings(double x, double y, double angle, Team_ID teamID) {
	this->powerReset();
	determineShootingAngles();
	updateAllValues(); //powerReset doesn't reset the level's doings

	this->dead = false;
	this->x = x;
	this->y = y;
	this->velocity = SimpleVector2D(angle, 0, true);
	this->gameID = GameManager::getNextID();
	this->teamID = teamID;
	//this->r = TANK_RADIUS;
	shootCount = 0;
	//don't update maxShootCount

	if (RNG::randFunc() < 1.0/4096) {
		//shiny tank (yes, 1/8192 is the chance before Sword/Shield)
		defaultColor = ColorValueHolder(0.75f, 0.75f, 0.75f);
	} else {
		defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f);
	}
}
*/

double Tank::getHighestOffenseImportance() const {
	double highest = LOW_IMPORTANCE;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getOffenseImportance() > highest) {
			highest = tankPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

double Tank::getHighestOffenseTier(double importance) const {
	double highest = LOW_TIER;
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

double Tank::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double Tank::getHighestDefenseImportance() const {
	double highest = LOW_IMPORTANCE;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getDefenseImportance() > highest) {
			highest = tankPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

double Tank::getHighestDefenseTier(double importance) const {
	double highest = LOW_TIER;
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

double Tank::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}
