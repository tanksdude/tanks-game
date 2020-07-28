#include "shotgunpower.h"
#include "shotguntankpower.h"
#include "shotgunbulletpower.h"

TankPower* ShotgunPower::makeTankPower() const {
	return new ShotgunTankPower();
}

BulletPower* ShotgunPower::makeBulletPower() const {
	return new ShotgunBulletPower();
}

/*
HazardPower* ShotgunPower::makeHazardPower() const {
	return new ShotgunHazardPower();
}
*/

Power* ShotgunPower::factory() {
	return new ShotgunPower();
}

ShotgunPower::ShotgunPower() {
	return;
}
