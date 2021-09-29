#include "devlonginvinciblenamedpower.h"

TankPower* DevLongInvincibleNamedPower::makeTankPower() const {
	return new DevLongInvincibleNamedTankPower();
}

BulletPower* DevLongInvincibleNamedPower::makeBulletPower() const {
	return new DevLongInvincibleNamedBulletPower();
}

/*
HazardPower* DevLongInvincibleNamedPower::makeHazardPower() const {
	return new DevLongInvincibleNamedHazardPower();
}
*/

Power* DevLongInvincibleNamedPower::factory() {
	return new DevLongInvincibleNamedPower();
}

DevLongInvincibleNamedPower::DevLongInvincibleNamedPower() {
	return;
}



void DevLongInvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void DevLongInvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* DevLongInvincibleNamedTankPower::makeBulletPower() const {
	return new DevLongInvincibleNamedBulletPower();
}

DevLongInvincibleNamedTankPower::DevLongInvincibleNamedTankPower() {
	maxTime = -1;
	timeLeft = 0;
}



void DevLongInvincibleNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevLongInvincibleNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevLongInvincibleNamedBulletPower::makeTankPower() const {
	return new DevLongInvincibleNamedTankPower();
}

DevLongInvincibleNamedBulletPower::DevLongInvincibleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
