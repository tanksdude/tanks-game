#pragma once
#include "homingtankpower.h"
#include "homingbulletpower.h"
#include "homingpower.h"

void HomingTankPower::initialize(Tank* parent) {
	//nothing
}
void HomingTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* HomingTankPower::makeBulletPower() {
	return new HomingBulletPower();
}

HomingTankPower::HomingTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
