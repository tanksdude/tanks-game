#include "bignamedpower.h"

std::unordered_map<std::string, float> BigNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	//weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

TankPower* BigNamedPower::makeTankPower() const {
	return new BigNamedTankPower();
}

BulletPower* BigNamedPower::makeBulletPower() const {
	return new BigNamedBulletPower();
}

/*
HazardPower* BigNamedPower::makeHazardPower() const {
	return new BigNamedHazardPower();
}
*/

Power* BigNamedPower::factory() {
	return new BigNamedPower();
}

BigNamedPower::BigNamedPower() {
	return;
}



void BigNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void BigNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BigNamedTankPower::makeBulletPower() const {
	return new BigNamedBulletPower();
}

BigNamedTankPower::BigNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



InteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

void BigNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BigNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* BigNamedBulletPower::makeTankPower() const {
	return new BigNamedTankPower();
}

BigNamedBulletPower::BigNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;

	bulletRadiusStacks = true;
}
