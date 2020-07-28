#include "speedtankpower.h"
#include "speedbulletpower.h"

void SpeedTankPower::initialize(Tank* parent) {
	//nothing
}

void SpeedTankPower::removeEffects(Tank* parent) {
	//nothing
}

double SpeedTankPower::getTankMaxSpeedMultiplier() const {
	return 2;
}

BulletPower* SpeedTankPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

SpeedTankPower::SpeedTankPower() {
	maxTime = 500;
	timeLeft = 500;

	tankMaxSpeedStacks = true;
}
