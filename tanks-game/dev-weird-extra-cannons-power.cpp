#include "dev-weird-extra-cannons-power.h"

std::unordered_map<std::string, float> DevWeirdExtraCannonsPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* DevWeirdExtraCannonsPower::makeTankPower() const {
	return new DevWeirdExtraCannonsTankPower();
}

BulletPower* DevWeirdExtraCannonsPower::makeBulletPower() const {
	return new DevWeirdExtraCannonsBulletPower();
}

/*
HazardPower* DevWeirdExtraCannonsPower::makeHazardPower() const {
	return new DevWeirdExtraCannonsHazardPower();
}
*/

Power* DevWeirdExtraCannonsPower::factory() {
	return new DevWeirdExtraCannonsPower();
}

DevWeirdExtraCannonsPower::DevWeirdExtraCannonsPower() {
	return;
}



void DevWeirdExtraCannonsTankPower::initialize(Tank* parent) {
	//nothing
}

void DevWeirdExtraCannonsTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* DevWeirdExtraCannonsTankPower::makeBulletPower() const {
	return new DevWeirdExtraCannonsBulletPower();
}

void DevWeirdExtraCannonsTankPower::addShootingPoints(Tank* t, std::vector<CannonPoint>* cannonPoints) {
	for (int i = cannonPoints->size() - 1; i >= 0; i--) {
		int end = (i + 1) % cannonPoints->size();
		double angle_diff;
		if (end == 0) {
			angle_diff = 2*PI - (cannonPoints->at(i).angle - cannonPoints->at(end).angle);
		} else {
			angle_diff = cannonPoints->at(end).angle - cannonPoints->at(i).angle;
		}

		double newAngle1 = (angle_diff * (1.0/4)); //90deg
		CannonPoint temp1 = CannonPoint(newAngle1 + cannonPoints->at(i).angle);
		cannonPoints->insert(cannonPoints->begin() + i + 1, temp1);

		double newAngle2 = (angle_diff * (5.0/8)); //180+45deg
		CannonPoint temp2 = CannonPoint(newAngle2 + cannonPoints->at(i).angle);
		cannonPoints->insert(cannonPoints->begin() + i + 2, temp2);
	}
}

DevWeirdExtraCannonsTankPower::DevWeirdExtraCannonsTankPower() {
	maxTime = 500;
	timeLeft = 500;

	addsShootingPoints = true;
}



void DevWeirdExtraCannonsBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevWeirdExtraCannonsBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevWeirdExtraCannonsBulletPower::makeTankPower() const {
	return new DevWeirdExtraCannonsTankPower();
}

DevWeirdExtraCannonsBulletPower::DevWeirdExtraCannonsBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
