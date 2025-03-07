#include "inversion-power.h"

std::unordered_map<std::string, float> InversionPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* InversionPower::makeTankPower() const {
	return new InversionTankPower();
}

BulletPower* InversionPower::makeBulletPower() const {
	return new InversionBulletPower();
}

Power* InversionPower::factory() {
	return new InversionPower();
}

InversionPower::InversionPower() {
	return;
}



BulletPower* InversionTankPower::makeBulletPower() const {
	return new InversionBulletPower();
}

InversionTankPower::InversionTankPower() {
	maxTime = 500;
	timeLeft = 500;

	tankTurningIncrementStacks = true;
}



TankPower* InversionBulletPower::makeTankPower() const {
	return new InversionTankPower();
}

InversionBulletPower::InversionBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
