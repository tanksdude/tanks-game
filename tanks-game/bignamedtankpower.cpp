#pragma once
#include "bignamedtankpower.h"
#include "bignamedbulletpower.h"
#include "bignamedpower.h"

void BigNamedTankPower::initialize(Tank* parent) {
	//nothing
}
void BigNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BigNamedTankPower::makeBulletPower() {
	return new BigNamedBulletPower();
}

BigNamedTankPower::BigNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;

	bulletRadiusStacks = true;
}
