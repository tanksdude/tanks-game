#include "shotguntankpower.h"
#include "shotgunbulletpower.h"
#include "constants.h"
#include <math.h>

const int ShotgunTankPower::bulletSpreadCount = 2;

void ShotgunTankPower::additionalShooting(Tank* parent, CannonPoint c) {
	double angleDiff = PI/2 /  bulletSpreadCount;
	for (int i = 1; i <= bulletSpreadCount; i++) {
		parent->regularMakeBullet(parent->r * cos(parent->angle + c.angle + angleDiff*i), parent->r * sin(parent->angle + c.angle + angleDiff*i), parent->angle + c.angle);
		parent->regularMakeBullet(parent->r * cos(parent->angle + c.angle - angleDiff*i), parent->r * sin(parent->angle + c.angle - angleDiff*i), parent->angle + c.angle);
	}
}

void ShotgunTankPower::initialize(Tank* parent) {
	//nothing
}

void ShotgunTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* ShotgunTankPower::makeBulletPower() {
	return new ShotgunBulletPower();
}

ShotgunTankPower::ShotgunTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
}