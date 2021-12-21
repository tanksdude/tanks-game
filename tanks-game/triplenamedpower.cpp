#include "triplenamedpower.h"

std::unordered_map<std::string, float> TripleNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* TripleNamedPower::makeTankPower() const {
	return new TripleNamedTankPower();
}

BulletPower* TripleNamedPower::makeBulletPower() const {
	return new TripleNamedBulletPower();
}

/*
HazardPower* TripleNamedPower::makeHazardPower() const {
	return new TripleNamedHazardPower();
}
*/

Power* TripleNamedPower::factory() {
	return new TripleNamedPower();
}

TripleNamedPower::TripleNamedPower() {
	return;
}



#include "constants.h"
#include <math.h>

const double TripleNamedTankPower::angleDiff = PI/16;

void TripleNamedTankPower::additionalShooting(Tank* parent, CannonPoint c) {
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle + angleDiff), parent->r * sin(parent->velocity.getAngle() + c.angle + angleDiff), parent->velocity.getAngle() + c.angle);
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle - angleDiff), parent->r * sin(parent->velocity.getAngle() + c.angle - angleDiff), parent->velocity.getAngle() + c.angle);
}

void TripleNamedTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void TripleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* TripleNamedTankPower::makeBulletPower() const {
	return new TripleNamedBulletPower();
}

TripleNamedTankPower::TripleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000

	modifiesAdditionalShooting = true;
}



void TripleNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void TripleNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* TripleNamedBulletPower::makeTankPower() const {
	return new TripleNamedTankPower();
}

TripleNamedBulletPower::TripleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
