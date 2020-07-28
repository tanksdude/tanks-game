#include "homingpower.h"
#include "homingtankpower.h"
#include "homingbulletpower.h"

TankPower* HomingPower::makeTankPower() const {
	return new HomingTankPower();
}

BulletPower* HomingPower::makeBulletPower() const {
	return new HomingBulletPower();
}

/*
HazardPower* HomingPower::makeHazardPower() const {
	return new HomingHazardPower();
}
*/

Power* HomingPower::factory() {
	return new HomingPower();
}

HomingPower::HomingPower() {
	return;
}
