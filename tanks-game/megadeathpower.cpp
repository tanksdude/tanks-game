#pragma once
#include "megadeathpower.h"
#include "megadeathtankpower.h"
#include "megadeathbulletpower.h"

const bool MegaDeathPower::canBeInARandomLevel = true;

TankPower* MegaDeathPower::makeTankPower() {
	return new MegaDeathTankPower();
}

BulletPower* MegaDeathPower::makeBulletPower() {
	return new MegaDeathBulletPower();
}

/*
HazardPower* MegaDeathPower::makeHazardPower(){
	return new MegaDeathHazardPower();
}
*/

Power* MegaDeathPower::factory() {
	return new MegaDeathPower();
}

MegaDeathPower::MegaDeathPower() {
	//nope
}

MegaDeathPower::~MegaDeathPower() {
	//unlikely to ever need these
}
