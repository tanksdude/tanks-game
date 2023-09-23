#include "triple-spread-named-power.h"

std::unordered_map<std::string, float> TripleSpreadNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* TripleSpreadNamedPower::makeTankPower() const {
	return new TripleSpreadNamedTankPower();
}

BulletPower* TripleSpreadNamedPower::makeBulletPower() const {
	return new TripleSpreadNamedBulletPower();
}

/*
HazardPower* TripleSpreadNamedPower::makeHazardPower() const {
	return new TripleSpreadNamedHazardPower();
}
*/

Power* TripleSpreadNamedPower::factory() {
	return new TripleSpreadNamedPower();
}

TripleSpreadNamedPower::TripleSpreadNamedPower() : TripleNamedPower() {
	return;
}



#include "../constants.h"
#include <cmath>

void TripleSpreadNamedTankPower::additionalShooting(Tank* parent, const CannonPoint& c) {
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle), parent->r * sin(parent->velocity.getAngle() + c.angle), parent->velocity.getAngle() + c.angle + TripleNamedPower::bulletAngleDiff);
	parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle), parent->r * sin(parent->velocity.getAngle() + c.angle), parent->velocity.getAngle() + c.angle - TripleNamedPower::bulletAngleDiff);
}

BulletPower* TripleSpreadNamedTankPower::makeBulletPower() const {
	return new TripleSpreadNamedBulletPower();
}

TripleSpreadNamedTankPower::TripleSpreadNamedTankPower() : TripleNamedTankPower() {
	//maxTime = 500;
	//timeLeft = 500;

	//modifiesAdditionalShooting = true;
}



TankPower* TripleSpreadNamedBulletPower::makeTankPower() const {
	return new TripleSpreadNamedTankPower();
}

TripleSpreadNamedBulletPower::TripleSpreadNamedBulletPower() : TripleNamedBulletPower() {
	//nothing new
}
