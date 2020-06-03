#pragma once
#include "speedtankpower.h"
#include "speedbulletpower.h"
#include "speedpower.h"

void SpeedTankPower::initialize(Tank* parent) {
	//parent->giveMaxSpeed() *= 2;
}
void SpeedTankPower::removeEffects(Tank* parent) {
	//parent->giveMaxSpeed() /= 2;
}

double SpeedTankPower::getTankMaxSpeedMultiplier() {
	return 2;
}

BulletPower* SpeedTankPower::makeBulletPower() {
	return new SpeedBulletPower();
}

SpeedTankPower::SpeedTankPower() {
	maxTime = 500;
	timeLeft = 500;

	tankMaxSpeedStacks = true;
}
