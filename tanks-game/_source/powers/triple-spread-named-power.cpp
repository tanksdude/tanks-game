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

Power* TripleSpreadNamedPower::factory() {
	return new TripleSpreadNamedPower();
}

TripleSpreadNamedPower::TripleSpreadNamedPower() : TripleNamedPower() {
	return;
}



std::vector<std::pair<double, double>>* TripleSpreadNamedTankPower::addExtraShootingPoints() const {
	return new std::vector<std::pair<double, double>>{ {0,  TripleNamedPower::bulletAngleDiff},
	                                                   {0, -TripleNamedPower::bulletAngleDiff} };
}

BulletPower* TripleSpreadNamedTankPower::makeBulletPower() const {
	return new TripleSpreadNamedBulletPower();
}

TripleSpreadNamedTankPower::TripleSpreadNamedTankPower() : TripleNamedTankPower() {
	addsExtraShootingPoints = true;
}



TankPower* TripleSpreadNamedBulletPower::makeTankPower() const {
	return new TripleSpreadNamedTankPower();
}

TripleSpreadNamedBulletPower::TripleSpreadNamedBulletPower() : TripleNamedBulletPower() {
	//nothing new
}
