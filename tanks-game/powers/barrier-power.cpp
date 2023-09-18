#include "barrier-power.h"

std::unordered_map<std::string, float> BarrierPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	return weights;
}

TankPower* BarrierPower::makeTankPower() const {
	return new BarrierTankPower();
}

BulletPower* BarrierPower::makeBulletPower() const {
	return new BarrierBulletPower();
}

/*
HazardPower* BarrierPower::makeHazardPower() const {
	return new BarrierHazardPower();
}
*/

Power* BarrierPower::factory() {
	return new BarrierPower();
}

BarrierPower::BarrierPower() {
	return;
}



void BarrierTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void BarrierTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BarrierTankPower::makeBulletPower() const {
	return new BarrierBulletPower();
}

InteractionBoolHolder BarrierTankPower::modifiedDeathHandling(Tank* parent) {
	return { false, true };
}

BarrierTankPower::BarrierTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000

	modifiesDeathHandling = true;
}



void BarrierBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BarrierBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* BarrierBulletPower::makeTankPower() const {
	return new BarrierTankPower();
}

InteractionBoolHolder BarrierBulletPower::modifiedDeathHandling(Bullet* parent) {
	return { false, true };
}

BarrierBulletPower::BarrierBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesDeathHandling = true;
}