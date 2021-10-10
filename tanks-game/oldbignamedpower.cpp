#include "oldbignamedpower.h"

std::unordered_map<std::string, float> OldBigNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
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

OldBigNamedPower::OldBigNamedPower() {
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

OldBigNamedTankPower::OldBigNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



InteractionBoolHolder OldBigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
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

OldBigNamedBulletPower::OldBigNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;

	bulletSpeedStacks = true;
	bulletRadiusStacks = true;
}
