#include "barrier-power.h"

std::unordered_map<std::string, float> BarrierPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "old", 0.5f });
	weights.insert({ "random-old", 0.5f });
	weights.insert({ "random", 0.125f });
	return weights;
}

TankPower* BarrierPower::makeTankPower() const {
	return new BarrierTankPower();
}

BulletPower* BarrierPower::makeBulletPower() const {
	return new BarrierBulletPower();
}

Power* BarrierPower::factory() {
	return new BarrierPower();
}

BarrierPower::BarrierPower() {
	return;
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
	//in JS, the tank's shooting cooldown was reset

	modifiesDeathHandling = true;
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
