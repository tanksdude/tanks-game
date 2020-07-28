#include "oldbignamedtankpower.h"
#include "oldbignamedbulletpower.h"

void OldBigNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void OldBigNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldBigNamedTankPower::makeBulletPower() const {
	return new OldBigNamedBulletPower();
}

OldBigNamedTankPower::OldBigNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
