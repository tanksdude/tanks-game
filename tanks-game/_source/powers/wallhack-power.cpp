#include "wallhack-power.h"

std::unordered_map<std::string, float> WallhackPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.25f }); //TODO: something like "reduced random-vanilla" to exclude speed, invincible, wallhack
	weights.insert({ "old", 1.0f });
	weights.insert({ "ultimate", 1.0f });
	weights.insert({ "ultimate-vanilla", 1.0f });
	weights.insert({ "random", 0.25f });
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
