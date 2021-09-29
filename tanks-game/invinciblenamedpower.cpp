#include "invinciblenamedpower.h"

TankPower* InvincibleNamedPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

BulletPower* InvincibleNamedPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

/*
HazardPower* InvincibleNamedPower::makeHazardPower() const {
	return new InvincibleNamedHazardPower();
}
*/

Power* InvincibleNamedPower::factory() {
	return new InvincibleNamedPower();
}

InvincibleNamedPower::InvincibleNamedPower() {
	return;
}



void InvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void InvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* InvincibleNamedTankPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

InvincibleNamedTankPower::InvincibleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



void InvincibleNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void InvincibleNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* InvincibleNamedBulletPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

InvincibleNamedBulletPower::InvincibleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
