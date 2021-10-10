#include "wallhackpower.h"

std::unordered_map<std::string, float> WallhackPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
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

InteractionBoolHolder WallhackBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, false };
}

WallhackBulletPower::WallhackBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
