#pragma once
#include "multishotpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"

const bool MultishotPower::canBeInARandomLevel = true;
//ColorValueHolder WallhackPower::classColor = ColorValueHolder(0x88, 0x00, 0xFF);

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