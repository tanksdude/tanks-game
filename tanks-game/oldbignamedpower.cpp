#include "oldbignamedpower.h"
#include "oldbignamedtankpower.h"
#include "oldbignamedbulletpower.h"

TankPower* OldBigNamedPower::makeTankPower() {
	return new OldBigNamedTankPower();
}

BulletPower* OldBigNamedPower::makeBulletPower() {
	return new OldBigNamedBulletPower();
}

/*
HazardPower* OldBigNamedPower::makeHazardPower(){
	return new OldBigNamedHazardPower();
}
*/

Power* OldBigNamedPower::factory() {
	return new OldBigNamedPower();
}

OldBigNamedPower::OldBigNamedPower() {
	return;
}
