#include "dev-other-stuff-is-poison-power.h"

std::unordered_map<std::string, float> DevOtherStuffIsPoisonPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 0.25f }); //it's supposed to be a test power, not actually used
	return weights;
}

TankPower* DevOtherStuffIsPoisonPower::makeTankPower() const {
	return new DevOtherStuffIsPoisonTankPower();
}

BulletPower* DevOtherStuffIsPoisonPower::makeBulletPower() const {
	return new DevOtherStuffIsPoisonBulletPower();
}

/*
HazardPower* DevOtherStuffIsPoisonPower::makeHazardPower() const {
	return new DevOtherStuffIsPoisonHazardPower();
}
*/

Power* DevOtherStuffIsPoisonPower::factory() {
	return new DevOtherStuffIsPoisonPower();
}

DevOtherStuffIsPoisonPower::DevOtherStuffIsPoisonPower() {
	return;
}



void DevOtherStuffIsPoisonTankPower::initialize(Tank* parent) {
	//nothing
}

void DevOtherStuffIsPoisonTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* DevOtherStuffIsPoisonTankPower::makeBulletPower() const {
	return new DevOtherStuffIsPoisonBulletPower();
}

DevOtherStuffIsPoisonTankPower::DevOtherStuffIsPoisonTankPower() {
	maxTime = 500;
	timeLeft = 500;

	//modifiesEdgeCollision = true;
	//overridesEdgeCollision = false;

	//modifiesCollisionWithTank = true;
	//overridesCollisionWithTank = false;

	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = false;

	overridesCollisionWithCircleHazard = false;

	overridesCollisionWithRectHazard = false;
}



void DevOtherStuffIsPoisonBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevOtherStuffIsPoisonBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevOtherStuffIsPoisonBulletPower::makeTankPower() const {
	return new DevOtherStuffIsPoisonTankPower();
}

DevOtherStuffIsPoisonBulletPower::DevOtherStuffIsPoisonBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
