#include "dev-backwards-movement-power.h"

std::unordered_map<std::string, float> DevBackwardsMovementPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 0.25f }); //TODO
	return weights;
}

TankPower* DevBackwardsMovementPower::makeTankPower() const {
	return new DevBackwardsMovementTankPower();
}

BulletPower* DevBackwardsMovementPower::makeBulletPower() const {
	return new DevBackwardsMovementBulletPower();
}

/*
HazardPower* DevBackwardsMovementPower::makeHazardPower() const {
	return new DevBackwardsMovementHazardPower();
}
*/

Power* DevBackwardsMovementPower::factory() {
	return new DevBackwardsMovementPower();
}

DevBackwardsMovementPower::DevBackwardsMovementPower() {
	return;
}



void DevBackwardsMovementTankPower::initialize(Tank* parent) {
	//nothing
}

void DevBackwardsMovementTankPower::removeEffects(Tank* parent) {
	//nothing
}

InteractionBoolHolder DevBackwardsMovementTankPower::modifiedMovement(Tank* t, bool forward, bool turnL, bool turnR, bool specialKey) {
	if (specialKey) {
		//TODO: I don't know how this should be implemented (it should do something with acceleration; use the tank's or store a vector?)
		t->x -= t->maxSpeed * cos(t->velocity.getAngle());
		t->y -= t->maxSpeed * sin(t->velocity.getAngle());
	}
	return { false };
}

BulletPower* DevBackwardsMovementTankPower::makeBulletPower() const {
	return new DevBackwardsMovementBulletPower();
}

DevBackwardsMovementTankPower::DevBackwardsMovementTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesMovement = true;
}



void DevBackwardsMovementBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevBackwardsMovementBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevBackwardsMovementBulletPower::makeTankPower() const {
	return new DevBackwardsMovementTankPower();
}

DevBackwardsMovementBulletPower::DevBackwardsMovementBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
