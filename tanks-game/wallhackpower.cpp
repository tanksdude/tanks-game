#include "wallhackpower.h"
#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"

const bool WallhackPower::canBeInARandomLevel = true;

TankPower* WallhackPower::makeTankPower() {
	return new WallhackTankPower();
}

BulletPower* WallhackPower::makeBulletPower() {
	return new WallhackBulletPower();
}

/*
HazardPower* WallhackPower::makeHazardPower(){
	return new WallhackHazardPower();
}
*/

Power* WallhackPower::factory() {
	return new WallhackPower();
}

WallhackPower::WallhackPower() {
	//does nothing
}

WallhackPower::~WallhackPower() {
	//no need
}
