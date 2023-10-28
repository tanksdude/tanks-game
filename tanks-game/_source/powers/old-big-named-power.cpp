#include "old-big-named-power.h"

std::unordered_map<std::string, float> OldBigNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 0.5f });
	return weights;
}

TankPower* OldBigNamedPower::makeTankPower() const {
	return new OldBigNamedTankPower();
}

BulletPower* OldBigNamedPower::makeBulletPower() const {
	return new OldBigNamedBulletPower();
}

/*
HazardPower* OldBigNamedPower::makeHazardPower() const {
	return new OldBigNamedHazardPower();
}
*/

Power* OldBigNamedPower::factory() {
	return new OldBigNamedPower();
}

OldBigNamedPower::OldBigNamedPower() : BigNamedPower() {
	return;
}



BulletPower* OldBigNamedTankPower::makeBulletPower() const {
	return new OldBigNamedBulletPower();
}

OldBigNamedTankPower::OldBigNamedTankPower() : BigNamedTankPower() {
	//nothing new
}



TankPower* OldBigNamedBulletPower::makeTankPower() const {
	return new OldBigNamedTankPower();
}

OldBigNamedBulletPower::OldBigNamedBulletPower() : BigNamedBulletPower() {
	bulletSpeedStacks = true;
}
