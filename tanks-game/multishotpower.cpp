#include "multishotpower.h"

std::unordered_map<std::string, float> MultishotPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "supermix", .5f });
	weights.insert({ "supermix-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

TankPower* MultishotPower::makeTankPower() const {
	return new MultishotTankPower();
}

BulletPower* MultishotPower::makeBulletPower() const {
	return new MultishotBulletPower();
}

/*
HazardPower* MultishotPower::makeHazardPower() const {
	return new MultishotHazardPower();
}
*/

Power* MultishotPower::factory() {
	return new MultishotPower();
}

MultishotPower::MultishotPower() {
	return;
}



#include "powerfunctionhelper.h"

void MultishotTankPower::initialize(Tank* parent) {
	//deals with cannon points; nothing to initialize
}

void MultishotTankPower::removeEffects(Tank* parent) {
	//regenerate cannon points?
}

BulletPower* MultishotTankPower::makeBulletPower() const {
	return new MultishotBulletPower();
}

void MultishotTankPower::addShootingPoints(Tank* t, std::vector<CannonPoint>* cannonPoints) {
	PowerFunctionHelper::equallySpacedCannonPoints(t, cannonPoints, MultishotPower::bulletCount);
}

MultishotTankPower::MultishotTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000

	addsShootingPoints = true;
}



void MultishotBulletPower::initialize(Bullet* parent) {
	//nothing
}

void MultishotBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* MultishotBulletPower::makeTankPower() const {
	return new MultishotTankPower();
}

MultishotBulletPower::MultishotBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
