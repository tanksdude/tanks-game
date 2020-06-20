#include "blasttankpower.h"
#include "blastbulletpower.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>

const double BlastTankPower::bulletAngleDeviation = PI/3;
const int BlastTankPower::bulletAmount = 16;

void BlastTankPower::initialize(Tank* parent) {
	//nothing
}

void BlastTankPower::removeEffects(Tank* parent) {
	//nothing
}

void BlastTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		double tempAngle = (randFunc2()*2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		t->regularMakeBullet(t->r * cos(c.angle + t->angle + tempAngle), t->r * sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle);
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
