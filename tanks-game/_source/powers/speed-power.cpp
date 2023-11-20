#include "speed-power.h"

std::unordered_map<std::string, float> SpeedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f }); //TODO: something like "reduced random-vanilla" to exclude speed, invincible, wallhack
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f }); //TODO: reduce?
	return weights;
}

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
	return;
}



BulletPower* SpeedTankPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

SpeedTankPower::SpeedTankPower() {
	maxTime = 500;
	timeLeft = 500;

	tankMaxSpeedStacks = true;
}



TankPower* SpeedBulletPower::makeTankPower() const {
	return new SpeedTankPower();
}

SpeedBulletPower::SpeedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
