#include "dev-colorless-long-invincible-named-power.h"

TankPower* DevColorlessLongInvincibleNamedPower::makeTankPower() const {
	return new DevColorlessLongInvincibleNamedTankPower();
}

BulletPower* DevColorlessLongInvincibleNamedPower::makeBulletPower() const {
	return new DevColorlessLongInvincibleNamedBulletPower();
}

/*
HazardPower* DevColorlessLongInvincibleNamedPower::makeHazardPower() const {
	return new DevColorlessLongInvincibleNamedHazardPower();
}
*/

Power* DevColorlessLongInvincibleNamedPower::factory() {
	return new DevColorlessLongInvincibleNamedPower();
}

DevColorlessLongInvincibleNamedPower::DevColorlessLongInvincibleNamedPower() : DevLongInvincibleNamedPower() {
	return;
}



BulletPower* DevColorlessLongInvincibleNamedTankPower::makeBulletPower() const {
	return new DevColorlessLongInvincibleNamedBulletPower();
}

DevColorlessLongInvincibleNamedTankPower::DevColorlessLongInvincibleNamedTankPower() : DevLongInvincibleNamedTankPower() {
	//nothing new
}



TankPower* DevColorlessLongInvincibleNamedBulletPower::makeTankPower() const {
	return new DevColorlessLongInvincibleNamedTankPower();
}

DevColorlessLongInvincibleNamedBulletPower::DevColorlessLongInvincibleNamedBulletPower() : DevLongInvincibleNamedBulletPower() {
	//nothing new
}
