#include "blasttankpower.h"
#include "blastbulletpower.h"
#include "mylib.h"
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
		//TODO: this power wasn't JS-ified; look into whether it's now possible
		double tempAngle = (randFunc2()*2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		double tempAcc = randFunc2() * -(maxBulletAcceleration - minBulletAcceleration) - minBulletAcceleration; //[0,1] * accDiff + min
		t->complexMakeBullet(t->r * cos(c.angle + t->angle + tempAngle), t->r * sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle, 1, 1, tempAcc);
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
