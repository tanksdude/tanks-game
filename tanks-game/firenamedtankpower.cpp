#pragma once
#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"
#include "firenamedpower.h"
#include "constants.h"
#include <math.h>

const double FireNamedTankPower::bulletAngleDeviation = PI/4;
const double FireNamedTankPower::maxBulletAcceleration = 3/32.0;
const double FireNamedTankPower::minBulletAcceleration = 1/32.0;
const int FireNamedTankPower::bulletAmount = 4;

void FireNamedTankPower::initialize(Tank* parent) {
	//nothing
}
void FireNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

void FireNamedTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		//fmod(rand(), 2) only outputs whole numbers as doubles due to rand() being a whole number in [0, 32768), so classic rand()/RAND_MAX is what I need
		double tempAngle = (double(rand()) / RAND_MAX * 2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		double tempAcc = (double(rand()) / RAND_MAX) * -(maxBulletAcceleration - minBulletAcceleration) - minBulletAcceleration; //[0,1] * accDiff + min
		t->makeBullet(t->x + t->r*cos(c.angle + t->angle + tempAngle), t->y + t->r*sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle, t->r * t->getBulletRadiusMultiplier(), t->maxSpeed * t->getBulletSpeedMultiplier(), tempAcc);
	}
}

BulletPower* FireNamedTankPower::makeBulletPower() {
	return new FireNamedBulletPower();
}

FireNamedTankPower::FireNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}
