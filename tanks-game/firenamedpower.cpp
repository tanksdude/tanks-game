#include "firenamedpower.h"
#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"

TankPower* FireNamedPower::makeTankPower() const {
	return new FireNamedTankPower();
}

BulletPower* FireNamedPower::makeBulletPower() const {
	return new FireNamedBulletPower();
}

/*
HazardPower* FireNamedPower::makeHazardPower() const {
	return new FireNamedHazardPower();
}
*/

Power* FireNamedPower::factory() {
	return new FireNamedPower();
}

FireNamedPower::FireNamedPower() {
	return;
}
