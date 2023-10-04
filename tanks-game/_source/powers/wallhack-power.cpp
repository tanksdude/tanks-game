#include "wallhack-power.h"

std::unordered_map<std::string, float> WallhackPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "ultimate", 1.0f });
	weights.insert({ "ultimate-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* WallhackPower::makeTankPower() const {
	return new WallhackTankPower();
}

BulletPower* WallhackPower::makeBulletPower() const {
	return new WallhackBulletPower();
}

/*
HazardPower* WallhackPower::makeHazardPower() const {
	return new WallhackHazardPower();
}
*/

Power* WallhackPower::factory() {
	return new WallhackPower();
}

WallhackPower::WallhackPower() {
	return;
}



void WallhackTankPower::initialize(Tank* parent) {
	//nothing
}

void WallhackTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* WallhackTankPower::makeBulletPower() const {
	return new WallhackBulletPower();
}

InteractionBoolHolder WallhackTankPower::modifiedCollisionWithWall(Tank* t, Wall* w) {
	return { false, false };
}

WallhackTankPower::WallhackTankPower() {
	maxTime = 250;
	timeLeft = 250;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}



void WallhackBulletPower::initialize(Bullet* parent) {
	//nothing
}

void WallhackBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* WallhackBulletPower::makeTankPower() const {
	return new WallhackTankPower();
}

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> WallhackBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	return { false, false, nullptr, nullptr };
}

WallhackBulletPower::WallhackBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
