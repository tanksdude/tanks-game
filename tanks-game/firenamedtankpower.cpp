#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>

const double FireNamedTankPower::bulletAngleDeviation = PI/4;
const int FireNamedTankPower::bulletAmount = 4;

void FireNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void FireNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

void FireNamedTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		double tempAngle = (randFunc2()+randFunc2() - 1) * bulletAngleDeviation; //[-1,1] * deviation
		t->regularMakeBullet(t->r * cos(c.angle + t->angle + tempAngle), t->r * sin(c.angle + t->angle + tempAngle), c.angle + t->angle + tempAngle);
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
