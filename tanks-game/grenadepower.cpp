#pragma once
#include "grenadepower.h"
#include "grenadetankpower.h"
#include "grenadebulletpower.h"

const bool GrenadePower::canBeInARandomLevel = true;

TankPower* GrenadePower::makeTankPower() {
	return new GrenadeTankPower();
}

BulletPower* GrenadePower::makeBulletPower() {
	return new GrenadeBulletPower();
}

/*
HazardPower* GrenadePower::makeHazardPower(){
	return new GrenadeHazardPower();
}
*/

Power* GrenadePower::factory() {
	return new GrenadePower();
}

GrenadePower::GrenadePower() {
	//nope
}

GrenadePower::~GrenadePower() {
	//unlikely to ever need these
}
