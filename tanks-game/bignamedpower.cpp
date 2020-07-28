#include "bignamedpower.h"
#include "bignamedtankpower.h"
#include "bignamedbulletpower.h"

TankPower* BigNamedPower::makeTankPower() const {
	return new BigNamedTankPower();
}

BulletPower* BigNamedPower::makeBulletPower() const {
	return new BigNamedBulletPower();
}

/*
HazardPower* BigNamedPower::makeHazardPower() const {
	return new BigNamedHazardPower();
}
*/

Power* BigNamedPower::factory() {
	return new BigNamedPower();
}

BigNamedPower::BigNamedPower() {
	return;
}
