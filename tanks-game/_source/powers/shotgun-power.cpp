#include "shotgun-power.h"

const int ShotgunPower::bulletSpreadCount = 2;

std::unordered_map<std::string, float> ShotgunPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

TankPower* ShotgunPower::makeTankPower() const {
	return new ShotgunTankPower();
}

BulletPower* ShotgunPower::makeBulletPower() const {
	return new ShotgunBulletPower();
}

/*
HazardPower* ShotgunPower::makeHazardPower() const {
	return new ShotgunHazardPower();
}
*/

Power* ShotgunPower::factory() {
	return new ShotgunPower();
}

ShotgunPower::ShotgunPower() {
	return;
}



#include "../constants.h"
#include <cmath>

void ShotgunTankPower::additionalShooting(Tank* parent, const CannonPoint& c) {
	const double angleDiff = (PI/2) / ShotgunPower::bulletSpreadCount;
	for (int i = 1; i <= ShotgunPower::bulletSpreadCount; i++) {
		parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle + angleDiff*i), parent->r * sin(parent->velocity.getAngle() + c.angle + angleDiff*i), parent->velocity.getAngle() + c.angle);
		parent->regularMakeBullet(parent->r * cos(parent->velocity.getAngle() + c.angle - angleDiff*i), parent->r * sin(parent->velocity.getAngle() + c.angle - angleDiff*i), parent->velocity.getAngle() + c.angle);
	}
}

void ShotgunTankPower::initialize(Tank* parent) {
	//nothing
}

void ShotgunTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* ShotgunTankPower::makeBulletPower() const {
	return new ShotgunBulletPower();
}

ShotgunTankPower::ShotgunTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
}



void ShotgunBulletPower::initialize(Bullet* parent) {
	//nothing
}

void ShotgunBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* ShotgunBulletPower::makeTankPower() const {
	return new ShotgunTankPower();
}

ShotgunBulletPower::ShotgunBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
