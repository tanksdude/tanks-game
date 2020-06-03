#pragma once
#include "bignamedpower.h"
#include "bignamedtankpower.h"
#include "bignamedbulletpower.h"

const bool BigNamedPower::canBeInARandomLevel = true;

TankPower* BigNamedPower::makeTankPower() {
	return new BigNamedTankPower();
}

BulletPower* BigNamedPower::makeBulletPower() {
	return new BigNamedBulletPower();
}

/*
HazardPower* BigNamedPower::makeHazardPower(){
	return new BigNamedHazardPower();
}
*/

Power* BigNamedPower::factory() {
	return new BigNamedPower();
}

BigNamedPower::BigNamedPower() {
	//nope
}

BigNamedPower::~BigNamedPower() {
	//unlikely to ever need these
}
