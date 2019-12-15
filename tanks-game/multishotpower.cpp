#pragma once
#include "multishotpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"

const bool MultishotPower::canBeInARandomLevel = true;

TankPower* MultishotPower::makeTankPower() {
	return new MultishotTankPower();
}

BulletPower* MultishotPower::makeBulletPower() {
	return new MultishotBulletPower();
}

/*
HazardPower* MultishotPower::makeHazardPower(){
	return new MultishotHazardPower();
}
*/

Power* MultishotPower::factory() {
	return new MultishotPower();
}

MultishotPower::MultishotPower() {
	//does nothing
}

MultishotPower::~MultishotPower() {
	//no need
}