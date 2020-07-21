#include "shotgunpower.h"
#include "shotguntankpower.h"
#include "shotgunbulletpower.h"

TankPower* ShotgunPower::makeTankPower() {
	return new ShotgunTankPower();
}

BulletPower* ShotgunPower::makeBulletPower() {
	return new ShotgunBulletPower();
}

/*
HazardPower* ShotgunPower::makeHazardPower(){
	return new ShotgunHazardPower();
}
*/

Power* ShotgunPower::factory() {
	return new ShotgunPower();
}

ShotgunPower::ShotgunPower() {
	return;
}
