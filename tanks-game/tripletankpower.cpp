#include "tripletankpower.h"
#include "triplebulletpower.h"
#include "constants.h"
#include <math.h>

const double TripleTankPower::angleDiff = PI/16;

void TripleTankPower::additionalShooting(Tank* parent, CannonPoint c) {
	parent->regularMakeBullet(parent->r * cos(parent->angle + c.angle + angleDiff), parent->r * sin(parent->angle + c.angle + angleDiff), parent->angle + c.angle);
	parent->regularMakeBullet(parent->r * cos(parent->angle + c.angle - angleDiff), parent->r * sin(parent->angle + c.angle - angleDiff), parent->angle + c.angle);
}

void TripleTankPower::initialize(Tank* parent) {
	//nothing
}

void TripleTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* TripleTankPower::makeBulletPower() {
	return new TripleBulletPower();
}

TripleTankPower::TripleTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
}
