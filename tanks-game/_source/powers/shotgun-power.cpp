#include "shotgun-power.h"

const int ShotgunPower::bulletSpreadCount = 2;

std::unordered_map<std::string, float> ShotgunPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", 0.5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 0.5f });
	weights.insert({ "random", 0.5f });
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

std::vector<std::pair<double, double>>* ShotgunTankPower::addExtraShootingPoints() const {
	std::vector<std::pair<double, double>>* newExtraCannonPoints = new std::vector<std::pair<double, double>>;
	newExtraCannonPoints->reserve(ShotgunPower::bulletSpreadCount * 2);
	const double angleDiff = (PI/2) / ShotgunPower::bulletSpreadCount;
	for (int i = 1; i <= ShotgunPower::bulletSpreadCount; i++) {
		newExtraCannonPoints->push_back({ i *  angleDiff, i * -angleDiff });
		newExtraCannonPoints->push_back({ i * -angleDiff, i *  angleDiff });
	}
	return newExtraCannonPoints;
}

BulletPower* ShotgunTankPower::makeBulletPower() const {
	return new ShotgunBulletPower();
}

ShotgunTankPower::ShotgunTankPower() {
	maxTime = 500;
	timeLeft = 500;

	addsExtraShootingPoints = true;
}



TankPower* ShotgunBulletPower::makeTankPower() const {
	return new ShotgunTankPower();
}

ShotgunBulletPower::ShotgunBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
