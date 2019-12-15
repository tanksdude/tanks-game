#pragma once
#include "tripletankpower.h"
#include "triplebulletpower.h"
#include "triplepower.h"
#include <math.h>
#include "constants.h"

void TripleTankPower::additionalShooting(Tank* parent, CannonPoint c) {
	parent->defaultMakeBullet(parent->x + parent->r*cos(parent->angle + c.angle + TriplePower::angleDiff), parent->y + parent->r*sin(parent->angle + c.angle + TriplePower::angleDiff), parent->angle + c.angle);
	parent->defaultMakeBullet(parent->x + parent->r*cos(parent->angle + c.angle - TriplePower::angleDiff), parent->y + parent->r*sin(parent->angle + c.angle - TriplePower::angleDiff), parent->angle + c.angle);
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
