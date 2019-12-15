#pragma once
#include "wallhackpower.h"
#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"

const bool WallhackPower::canBeInARandomLevel = true;
//ColorValueHolder WallhackPower::classColor = ColorValueHolder(0xFF, 0xFF, 0x00);

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