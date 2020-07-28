#include "wallhackpower.h"
#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"

TankPower* WallhackPower::makeTankPower() const {
	return new WallhackTankPower();
}

BulletPower* WallhackPower::makeBulletPower() const {
	return new WallhackBulletPower();
}

/*
HazardPower* WallhackPower::makeHazardPower() const {
	return new WallhackHazardPower();
}
*/

Power* WallhackPower::factory() {
	return new WallhackPower();
}

WallhackPower::WallhackPower() {
	return;
}
