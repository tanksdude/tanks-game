#pragma once
#include "blasttankpower.h"
#include "blastbulletpower.h"
#include "blastpower.h"
#include "constants.h"
#include <math.h>

const double BlastTankPower::bulletAngleDeviation = PI/3;
const double BlastTankPower::maxBulletAcceleration = 3/16.0;
const double BlastTankPower::minBulletAcceleration = 1/16.0;
const int BlastTankPower::bulletAmount = 16;

void BlastTankPower::initialize(Tank* parent) {
	//nothing
}
void BlastTankPower::removeEffects(Tank* parent) {
	//nothing
}

void BlastTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		//fmod(rand(), 2) only outputs whole numbers as doubles due to rand() being a whole number in [0, 32768), so classic rand()/RAND_MAX is what I need
		double tempAngle = (double(rand()) / RAND_MAX * 2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		double tempAcc = (double(rand()) / RAND_MAX) * -(maxBulletAcceleration - minBulletAcceleration) - minBulletAcceleration; //[0,1] * accDiff + min
		t->makeBullet(t->x + t->r*cos(c.angle + t->angle + tempAngle), t->y + t->r*sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle, t->r * t->getBulletRadiusMultiplier(), t->maxSpeed * t->getBulletSpeedMultiplier(), tempAcc);
	}
}

BulletPower* BlastTankPower::makeBulletPower() {
	return new BlastBulletPower();
}

BlastTankPower::BlastTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}
