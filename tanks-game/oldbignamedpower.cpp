#include "oldbignamedpower.h"
#include "oldbignamedtankpower.h"
#include "oldbignamedbulletpower.h"

TankPower* OldBigNamedPower::makeTankPower() const {
	return new OldBigNamedTankPower();
}

BulletPower* OldBigNamedPower::makeBulletPower() const {
	return new OldBigNamedBulletPower();
}

/*
HazardPower* OldBigNamedPower::makeHazardPower() const {
	return new OldBigNamedHazardPower();
}
*/

Power* OldBigNamedPower::factory() {
	return new OldBigNamedPower();
}

OldBigNamedPower::OldBigNamedPower() {
	return;
}
