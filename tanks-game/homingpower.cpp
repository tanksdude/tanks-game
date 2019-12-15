#pragma once
#include "homingpower.h"
#include "homingtankpower.h"
#include "homingbulletpower.h"

const bool HomingPower::canBeInARandomLevel = true;

TankPower* HomingPower::makeTankPower() {
	return new HomingTankPower();
}

BulletPower* HomingPower::makeBulletPower() {
	return new HomingBulletPower();
}

/*
HazardPower* HomingPower::makeHazardPower(){
	return new HomingHazardPower();
}
*/

Power* HomingPower::factory() {
	return new HomingPower();
}

HomingPower::HomingPower() {
	//nope
}

HomingPower::~HomingPower() {
	//unlikely to ever need these
}