#pragma once
#include "bouncepower.h"
#include "bouncetankpower.h"
#include "bouncebulletpower.h"

const bool BouncePower::canBeInARandomLevel = true;
//ColorValueHolder BouncePower::classColor = ColorValueHolder(0xFF, 0xFF, 0x00);

TankPower* BouncePower::makeTankPower() {
	return new BounceTankPower();
}

BulletPower* BouncePower::makeBulletPower() {
	return new BounceBulletPower();
}

/*
HazardPower* BouncePower::makeHazardPower(){
	return new BounceHazardPower();
}
*/

Power* BouncePower::factory() {
	return new BouncePower();
}

BouncePower::BouncePower() {
	//nope
}

BouncePower::~BouncePower() {
	//unlikely to ever need these
}