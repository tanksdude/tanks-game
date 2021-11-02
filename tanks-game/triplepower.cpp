#include "triplepower.h"

TankPower* TriplePower::makeTankPower() const {
	return new TripleTankPower();
}

BulletPower* TriplePower::makeBulletPower() const {
	return new TripleBulletPower();
}

/*
HazardPower* TriplePower::makeHazardPower() const {
	return new TripleHazardPower();
}
*/

Power* TriplePower::factory() {
	return new TriplePower();
}

TriplePower::TriplePower() {
	return;
}



#include "constants.h"
#include <math.h>

const double TripleTankPower::angleDiff = PI/16;

void TripleTankPower::additionalShooting(Tank* parent, CannonPoint c) {
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle + angleDiff), parent->r * sin(parent->velocity.getAngle() + c.angle + angleDiff), parent->velocity.getAngle() + c.angle);
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle - angleDiff), parent->r * sin(parent->velocity.getAngle() + c.angle - angleDiff), parent->velocity.getAngle() + c.angle);
}

void TripleTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void TripleTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* TripleTankPower::makeBulletPower() const {
	return new TripleBulletPower();
}

TripleTankPower::TripleTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
}



void TripleBulletPower::initialize(Bullet* parent) {
	//nothing
}

void TripleBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* TripleBulletPower::makeTankPower() const {
	return new TripleTankPower();
}

TripleBulletPower::TripleBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
