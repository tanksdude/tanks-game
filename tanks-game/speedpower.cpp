#include "speedpower.h"

TankPower* SpeedPower::makeTankPower() const {
	return new SpeedTankPower();
}

BulletPower* SpeedPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

/*
HazardPower* SpeedPower::makeHazardPower() const {
	return new SpeedHazardPower();
}
*/

Power* SpeedPower::factory() {
	return new SpeedPower();
}

SpeedPower::SpeedPower() {
	//does nothing
}



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



void SpeedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void SpeedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* SpeedBulletPower::makeTankPower() const {
	return new SpeedTankPower();
}

SpeedBulletPower::SpeedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
