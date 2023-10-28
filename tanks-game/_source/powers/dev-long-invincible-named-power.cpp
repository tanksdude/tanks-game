#include "dev-long-invincible-named-power.h"

std::unordered_map<std::string, float> DevLongInvincibleNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	return weights;
}

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

DevLongInvincibleNamedPower::DevLongInvincibleNamedPower() : InvincibleNamedPower() {
	return;
}



BulletPower* DevLongInvincibleNamedTankPower::makeBulletPower() const {
	return new DevLongInvincibleNamedBulletPower();
}

DevLongInvincibleNamedTankPower::DevLongInvincibleNamedTankPower() : InvincibleNamedTankPower() {
	maxTime = -1;
	timeLeft = 0;
}



TankPower* DevLongInvincibleNamedBulletPower::makeTankPower() const {
	return new DevLongInvincibleNamedTankPower();
}

DevLongInvincibleNamedBulletPower::DevLongInvincibleNamedBulletPower() : InvincibleNamedBulletPower() {
	//nothing new
}
