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



void OldBigNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void OldBigNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldBigNamedTankPower::makeBulletPower() const {
	return new OldBigNamedBulletPower();
}

OldBigNamedTankPower::OldBigNamedTankPower() : BigNamedTankPower() {
	//nothing new
}



void OldBigNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldBigNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* OldBigNamedBulletPower::makeTankPower() const {
	return new OldBigNamedTankPower();
}

OldBigNamedBulletPower::OldBigNamedBulletPower() : BigNamedBulletPower() {
	bulletSpeedStacks = true;
}
