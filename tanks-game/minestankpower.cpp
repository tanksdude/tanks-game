#include "minestankpower.h"
#include "minesbulletpower.h"
#include "constants.h"
#include <math.h>

const double MinesTankPower::bulletDistance = 1.0/8;

void MinesTankPower::tick(Tank* t) {
	for (int i = 0; i < t->tankPowers.size(); i++) {
		if (t->tankPowers[i] != this) {
			if (t->tankPowers[i]->modifiesAdditionalShooting) {
				//might also want to check addsShootingPoints
				this->modifiesAdditionalShooting = false;
				return;
			}
		}
	}
	this->modifiesAdditionalShooting = true;
}

void MinesTankPower::additionalShooting(Tank* t, CannonPoint c) {
	t->regularMakeBullet(t->r * cos(t->angle + c.angle) * bulletDistance, t->r * sin(t->angle + c.angle) * bulletDistance, c.angle + t->angle);
}

void MinesTankPower::initialize(Tank* parent) {
	//nothing
}

void MinesTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* MinesTankPower::makeBulletPower() const {
	return new MinesBulletPower();
}

MinesTankPower::MinesTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}
