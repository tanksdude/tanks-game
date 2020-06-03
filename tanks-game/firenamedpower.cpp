#pragma once
#include "firenamedpower.h"
#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"

const bool FireNamedPower::canBeInARandomLevel = true;

TankPower* FireNamedPower::makeTankPower() {
	return new FireNamedTankPower();
}

BulletPower* FireNamedPower::makeBulletPower() {
	return new FireNamedBulletPower();
}

/*
HazardPower* FireNamedPower::makeHazardPower(){
	return new FireNamedHazardPower();
}
*/

Power* FireNamedPower::factory() {
	return new FireNamedPower();
}

FireNamedPower::FireNamedPower() {
	//nope
}

FireNamedPower::~FireNamedPower() {
	//unlikely to ever need these
}
