#include "tank.h"
#include "game-manager.h"
#include "constants.h"
#include <cmath>
#include "mylib.h"
#include "color-mixer.h"
#include "background-rect.h"
#include "renderer.h"
#include "bullet-manager.h"
#include "rng.h"
#include "level-manager.h"
#include <algorithm> //std::sort
#include <iostream>

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

/*
struct TankInputChar {
protected:
	std::string key;
	bool isSpecial;
	int key_num;
public:
	std::string getKey() const { return key; }
	bool getKeyState() const;
	TankInputChar(std::string);
	//TankInputChar(bool, int);
	TankInputChar();
};

TankInputChar::TankInputChar(std::string key_input) {
	key = key_input;
	if (KeypressManager::keyIsSpecialFromString(key_input)) {
		isSpecial = true;
		key_num = KeypressManager::specialKeyFromString(key_input);
	} else {
		isSpecial = false;
		key_num = KeypressManager::normalKeyFromString(key_input);
	}
}
TankInputChar::TankInputChar() {
	key = "`";
	isSpecial = false;
	key_num = '`';
}

bool TankInputChar::getKeyState() const {
	if (isSpecial) {
		return KeypressManager::getSpecialKey(key_num);
	}
	return KeypressManager::getNormalKey(key_num);
}
*/

VertexArray* Tank::va;
VertexBuffer* Tank::vb;
IndexBuffer* Tank::ib;
VertexArray* Tank::cannon_va;
VertexBuffer* Tank::cannon_vb;
bool Tank::initialized_GPU = false;

const double Tank::default_maxSpeed = 1;
const double Tank::default_acceleration = 1.0/16;
const double Tank::default_turningIncrement = 64;

Tank::Tank(double x_, double y_, double angle, Team_ID teamID, std::string name_, double shootCooldown) : GameThing(teamID) {
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
	maxShootCount = shootCooldown;

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

	float positions[(Circle::numOfSides+1) * (2+4)];
	positions[0] = 0;
	positions[1] = 0;
	positions[2] = positions[3] = positions[4] = 0.5f;
	positions[5] = 1.0f;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*6]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+1] = sin((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+2] = 0.5f;
		positions[i*6+3] = 0.5f;
		positions[i*6+4] = 0.5f;
		positions[i*6+5] = 1.0f;
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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*(2+4) * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vb->SetLayout(layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	va = VertexArray::MakeVertexArray();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	float cannon_positions[(2+4)*2] = {
		0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f
	};
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, (2+4)*2 * sizeof(float));
	VertexBufferLayout cannon_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	cannon_vb->SetLayout(layout);

	cannon_va = VertexArray::MakeVertexArray();
	cannon_va->AddVertexBuffer(cannon_vb);

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

bool Tank::move(bool forward, bool turnL, bool turnR, bool specialKey) {
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

			InteractionBoolHolder check_temp = tankPowers[k]->modifiedMovement(this, forward, turnL, turnR, specialKey);
			if (check_temp.shouldDie) {
				shouldBeKilled = true;
				overridedMovement = true;
				break;
			}
		}
	}

	if (!overridedMovement) {
		move_base(forward, turnL, turnR);
	}

	return shouldBeKilled;
}

inline void Tank::move_base(bool forward, bool turnL, bool turnR) {
	//TODO: ini settings vvv
	//if (!forward || !document.getElementById("moveturn").checked) { //change && to || and remove second ! to flip playstyle
		if (turnL) {
			velocity.changeAngle(PI / turningIncrement);
		}
		if (turnR) {
			velocity.changeAngle(-PI / turningIncrement);
		}
	//}
	//if (!document.getElementById("acceleration").checked) {
		if (forward) {
			velocity.changeMagnitude(acceleration);
		} else {
			velocity.changeMagnitude(-acceleration); //can result in negative velocities, fixed by SimpleVector2D just not allowing that
		}
		terminalVelocity(forward);
	//}
	/*else {
		if (forward)
			velocity = maxSpeed;
		else
			velocity = 0;
	}*/

	x += velocity.getXComp();
	y += velocity.getYComp();
}

inline void Tank::terminalVelocity(bool forward) {
	if (velocity.getMagnitude() > maxSpeed + acceleration) {
		velocity.changeMagnitude(-acceleration);
		if (forward && velocity.getMagnitude() > maxSpeed) {
			//so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could (maybe change?)
			velocity.changeMagnitude(-acceleration);
		}
	} else if (velocity.getMagnitude() > maxSpeed) {
		velocity.setMagnitude(maxSpeed);
	} /*else if (velocity.getMagnitude() < 0) {
		velocity.setMagnitude(0);
	}*/
}

void Tank::shoot(bool shooting) {
	//TODO: allow it to handle multiple shooting cooldowns? (not the point of the game (if it was, shooting cooldown color = mix(white, power color)))
	if (shootCount > 0) { //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;
	}

	if (shooting && shootCount <= 0) {
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
	//TODO: separate into power tick and power count down
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
	glm::mat4 modelMatrix;

	if (this->dead) {
		/*
		ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);

		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*va, *ib, *shader);
		*/

		ColorValueHolder deadColor = ColorValueHolder(0.0f, 0.0f, 0.0f);
		ColorValueHolder outerColor = ColorValueHolder(1.0f, 1.0f, 1.0f);
		deadColor = ColorMixer::mix(BackgroundRect::getBackColor(), deadColor, alpha);
		outerColor = ColorMixer::mix(BackgroundRect::getBackColor(), outerColor, alpha);

		float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
		coordsAndColor[0] = x;
		coordsAndColor[1] = y;
		coordsAndColor[2] = deadColor.getRf();
		coordsAndColor[3] = deadColor.getGf();
		coordsAndColor[4] = deadColor.getBf();
		coordsAndColor[5] = deadColor.getAf();
		for (int i = 1; i < Circle::numOfSides+1; i++) {
			coordsAndColor[i*6]   = x + r * cos((i-1) * 2*PI / Circle::numOfSides);
			coordsAndColor[i*6+1] = y + r * sin((i-1) * 2*PI / Circle::numOfSides);
			coordsAndColor[i*6+2] = outerColor.getRf();
			coordsAndColor[i*6+3] = outerColor.getGf();
			coordsAndColor[i*6+4] = outerColor.getBf();
			coordsAndColor[i*6+5] = outerColor.getAf();
		}

		unsigned int indices[Circle::numOfSides*3];
		for (int i = 0; i < Circle::numOfSides; i++) {
			indices[i*3]   = 0;
			indices[i*3+1] = i+1;
			indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), indices, Circle::numOfSides*3);
	} else {
		if (tankPowers.size() <= 1) {
			/*
			ColorValueHolder color = getBodyColor();

			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
			shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

			modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
			shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

			Renderer::Draw(*va, *ib, *shader);
			*/

			ColorValueHolder color = getBodyColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (int i = 1; i < Circle::numOfSides+1; i++) {
				coordsAndColor[i*6]   = x + r * cos((i-1) * 2*PI / Circle::numOfSides);
				coordsAndColor[i*6+1] = y + r * sin((i-1) * 2*PI / Circle::numOfSides);
				coordsAndColor[i*6+2] = color.getRf();
				coordsAndColor[i*6+3] = color.getGf();
				coordsAndColor[i*6+4] = color.getBf();
				coordsAndColor[i*6+5] = color.getAf();
			}

			unsigned int indices[Circle::numOfSides*3];
			for (int i = 0; i < Circle::numOfSides; i++) {
				indices[i*3]   = 0;
				indices[i*3+1] = i+1;
				indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), indices, Circle::numOfSides*3);
		} else {
			//main color split:
			/*
			ColorValueHolder color;
			for (int i = 0; i < tankPowers.size(); i++) {
				color = tankPowers[i]->getColor();

				color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
				shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

				double rotatePercent = floor((float(i) / tankPowers.size()) * Circle::numOfSides) / Circle::numOfSides;
				double nextRotatePercent = floor((float(i+1) / tankPowers.size()) * Circle::numOfSides) / Circle::numOfSides;
				unsigned int rotateVertices = floor((nextRotatePercent - rotatePercent) * Circle::numOfSides);
				modelMatrix = Renderer::GenerateModelMatrix(r, r, velocity.getAngle() + (rotatePercent * 2*PI), x, y);
				shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

				Renderer::Draw(*va, *ib, *shader, rotateVertices*3);
			}
			*/

			ColorValueHolder color;

			for (int i = 0; i < tankPowers.size(); i++) {
				std::vector<float> coordsAndColor_colorSplit;
				std::vector<unsigned int> indices_colorSplit;
				//could just use an array, since the size should be easy to calculate, but that's effort
				//could push everything to an array then only submit one batched draw call, but that's more effort

				color = tankPowers[i]->getColor();
				color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

				double rotatePercent = floor((float(i) / tankPowers.size()) * Circle::numOfSides) / Circle::numOfSides;
				double nextRotatePercent = floor((float(i+1) / tankPowers.size()) * Circle::numOfSides) / Circle::numOfSides;
				//unsigned int rotateVertices = floor((nextRotatePercent - rotatePercent) * Circle::numOfSides);
				unsigned int rotateVertexStart = floor(rotatePercent * Circle::numOfSides);
				unsigned int rotateVertexEnd = floor(nextRotatePercent * Circle::numOfSides);

				coordsAndColor_colorSplit.push_back(x);
				coordsAndColor_colorSplit.push_back(y);
				coordsAndColor_colorSplit.push_back(color.getRf());
				coordsAndColor_colorSplit.push_back(color.getGf());
				coordsAndColor_colorSplit.push_back(color.getBf());
				coordsAndColor_colorSplit.push_back(color.getAf());
				for (int i = rotateVertexStart; i <= rotateVertexEnd; i++) {
					coordsAndColor_colorSplit.push_back(x + r * cos(velocity.getAngle() + (i * 2*PI / Circle::numOfSides)));
					coordsAndColor_colorSplit.push_back(y + r * sin(velocity.getAngle() + (i * 2*PI / Circle::numOfSides)));
					coordsAndColor_colorSplit.push_back(color.getRf());
					coordsAndColor_colorSplit.push_back(color.getGf());
					coordsAndColor_colorSplit.push_back(color.getBf());
					coordsAndColor_colorSplit.push_back(color.getAf());
				}

				for (int i = 0; i < (rotateVertexEnd - rotateVertexStart); i++) {
					indices_colorSplit.push_back(0);
					indices_colorSplit.push_back(i+1);
					indices_colorSplit.push_back(i+2);
					//TODO: this doesn't feel right; verify later
				}

				Renderer::SubmitBatchedDraw(coordsAndColor_colorSplit.data(), coordsAndColor_colorSplit.size(), indices_colorSplit.data(), indices_colorSplit.size());
			}

			//center colors mix:
			/*
			color = getBodyColor();

			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
			shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

			modelMatrix = Renderer::GenerateModelMatrix(r*.75, r*.75, 0, x, y);
			shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

			Renderer::Draw(*va, *ib, *shader);
			*/

			color = getBodyColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (int i = 1; i < Circle::numOfSides+1; i++) {
				coordsAndColor[i*6]   = x + (r*.75) * cos((i-1) * 2*PI / Circle::numOfSides);
				coordsAndColor[i*6+1] = y + (r*.75) * sin((i-1) * 2*PI / Circle::numOfSides);
				coordsAndColor[i*6+2] = color.getRf();
				coordsAndColor[i*6+3] = color.getGf();
				coordsAndColor[i*6+4] = color.getBf();
				coordsAndColor[i*6+5] = color.getAf();
			}

			unsigned int indices[Circle::numOfSides*3];
			for (int i = 0; i < Circle::numOfSides; i++) {
				indices[i*3]   = 0;
				indices[i*3+1] = i+1;
				indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), indices, Circle::numOfSides*3);
		}
	}
}

inline void Tank::drawDead(float alpha) const {
	//TODO: draw X like no bullet zone
	drawBody(alpha);
	drawOutline(alpha);
}

inline void Tank::drawOutline(float alpha) const {
	return;

	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	ColorValueHolder color;
	if (this->dead) {
		color = ColorValueHolder(1.0f, 1.0f, 1.0f);
	} else {
		color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	}
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void Tank::drawShootingCooldown(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	double shootingOutlinePercent;
	if (maxShootCount*getShootingSpeedMultiplier() <= 0 || maxShootCount <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = constrain<double>(shootCount/(maxShootCount*getShootingSpeedMultiplier()), 0, 1);
	}
	unsigned int shootingOutlineTriangles = Circle::numOfSides * shootingOutlinePercent;

	/*
	if (shootingOutlineVertices > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		modelMatrix = Renderer::GenerateModelMatrix(r * 5.0/4.0, r * 5.0/4.0, getAngle(), x, y);
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*va, *ib, *shader, shootingOutlineVertices*3);
	}
	*/

	if (shootingOutlineTriangles > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

		float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
		coordsAndColor[0] = x;
		coordsAndColor[1] = y;
		coordsAndColor[2] = color.getRf();
		coordsAndColor[3] = color.getGf();
		coordsAndColor[4] = color.getBf();
		coordsAndColor[5] = color.getAf();
		for (int i = 0; i <= shootingOutlineTriangles && i < Circle::numOfSides; i++) {
			coordsAndColor[(i+1)*6]   = x + (r*(5.0/4.0)) * cos(velocity.getAngle() + i * 2*PI / Circle::numOfSides);
			coordsAndColor[(i+1)*6+1] = y + (r*(5.0/4.0)) * sin(velocity.getAngle() + i * 2*PI / Circle::numOfSides);
			coordsAndColor[(i+1)*6+2] = color.getRf();
			coordsAndColor[(i+1)*6+3] = color.getGf();
			coordsAndColor[(i+1)*6+4] = color.getBf();
			coordsAndColor[(i+1)*6+5] = color.getAf();
		}

		unsigned int indices[Circle::numOfSides*3];
		for (int i = 0; i < shootingOutlineTriangles; i++) {
			indices[i*3]   = 0;
			indices[i*3+1] = i+1;
			indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (shootingOutlineTriangles < Circle::numOfSides ? (shootingOutlineTriangles+2)*(2+4) : (shootingOutlineTriangles+1)*(2+4)), indices, shootingOutlineTriangles*3);
	}
}

inline void Tank::drawPowerCooldown(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//first, sort by timeLeft/maxTime
	/*
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
	*/
	std::vector<TankPower*> sortedTankPowers(tankPowers);
	std::sort(sortedTankPowers.begin(), sortedTankPowers.end(),
		[](const TankPower* lhs, const TankPower* rhs) { return (lhs->timeLeft/lhs->maxTime > rhs->timeLeft/rhs->maxTime); });

	//second, actually draw them
	for (int i = 0; i < sortedTankPowers.size(); i++) {
		double powerOutlinePercent;
		if (sortedTankPowers[i]->maxTime <= 0) {
			powerOutlinePercent = 0;
		} else {
			powerOutlinePercent = constrain<double>(sortedTankPowers[i]->timeLeft/sortedTankPowers[i]->maxTime, 0, 1);
		}
		unsigned int powerOutlineTriangles = Circle::numOfSides * powerOutlinePercent;

		/*
		if (powerOutlineVertices > 0) {
			ColorValueHolder c = sortedTankPowers[i]->getColor();
			c = ColorMixer::mix(BackgroundRect::getBackColor(), c, alpha);
			shader->setUniform4f("u_color", c.getRf(), c.getGf(), c.getBf(), c.getAf());

			modelMatrix = Renderer::GenerateModelMatrix(r * 9.0/8.0, r * 9.0/8.0, getAngle(), x, y);
			shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

			Renderer::Draw(*va, *ib, *shader, powerOutlineVertices*3);
		}
		*/

		if (powerOutlineTriangles > 0) {
			ColorValueHolder color = sortedTankPowers[i]->getColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (int i = 0; i <= powerOutlineTriangles && i < Circle::numOfSides; i++) {
				coordsAndColor[(i+1)*6]   = x + (r*(9.0/8.0)) * cos(velocity.getAngle() + i * 2*PI / Circle::numOfSides);
				coordsAndColor[(i+1)*6+1] = y + (r*(9.0/8.0)) * sin(velocity.getAngle() + i * 2*PI / Circle::numOfSides);
				coordsAndColor[(i+1)*6+2] = color.getRf();
				coordsAndColor[(i+1)*6+3] = color.getGf();
				coordsAndColor[(i+1)*6+4] = color.getBf();
				coordsAndColor[(i+1)*6+5] = color.getAf();
			}

			unsigned int indices[Circle::numOfSides*3];
			for (int i = 0; i < powerOutlineTriangles; i++) {
				indices[i*3]   = 0;
				indices[i*3+1] = i+1;
				indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (powerOutlineTriangles < Circle::numOfSides ? (powerOutlineTriangles+2)*(2+4) : (powerOutlineTriangles+1)*(2+4)), indices, powerOutlineTriangles*3);
		}
	}
}

inline void Tank::drawMainBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, 1, getAngle(), x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void Tank::drawExtraBarrels(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	//shader->setUniform4f("u_color", 0.5f, 0.5f, 0.5f, 0.25f); //CPU color
	ColorValueHolder color = ColorValueHolder(0.75f, 0.75f, 0.75f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	for (int i = 1; i < shootingPoints.size(); i++) {
		modelMatrix = Renderer::GenerateModelMatrix(r, 1, getRealCannonAngle(i), x, y);
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

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
