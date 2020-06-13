#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"
#include "mylib.h"
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
		//TODO: this power wasn't JS-ified; look into whether it's now possible
		double tempAngle = (randFunc2()*2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		double tempAcc = randFunc2() * -(maxBulletAcceleration - minBulletAcceleration) - minBulletAcceleration; //[0,1] * accDiff + min
		t->complexMakeBullet(t->r * cos(c.angle + t->angle + tempAngle), t->r * sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle, 1, 1, tempAcc);
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
