#include "triple-named-power.h"

const double TripleNamedPower::bulletAngleDiff = PI/16; //JS: PI/8

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



#include "../constants.h"
#include <cmath>

std::vector<std::pair<double, double>>* TripleNamedTankPower::addExtraShootingPoints() const {
	return new std::vector<std::pair<double, double>>{ {TripleNamedPower::bulletAngleDiff, -TripleNamedPower::bulletAngleDiff},
	                                                   {-TripleNamedPower::bulletAngleDiff, TripleNamedPower::bulletAngleDiff} };
}

BulletPower* TripleNamedTankPower::makeBulletPower() const {
	return new TripleNamedBulletPower();
}

TripleNamedTankPower::TripleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000
	//in JS, the tank's shooting cooldown was reset

	addsExtraShootingPoints = true;
}



TankPower* TripleNamedBulletPower::makeTankPower() const {
	return new TripleNamedTankPower();
}

TripleNamedBulletPower::TripleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
