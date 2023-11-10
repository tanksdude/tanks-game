#include "dev-color-changing-power.h"

#include "../constants.h"

#include "../tank-manager.h"

std::unordered_map<std::string, float> DevColorChangingPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 0.5f }); //doesn't do a whole lot
	return weights;
}

ColorValueHolder DevColorChangingPower::getColor() const {
	return DevColorChangingPower::getClassColor();
}

TankPower* DevColorChangingPower::makeTankPower() const {
	return new DevColorChangingTankPower();
}

BulletPower* DevColorChangingPower::makeBulletPower() const {
	return new DevColorChangingBulletPower();
}

/*
HazardPower* DevColorChangingPower::makeHazardPower() const {
	return new DevColorChangingHazardPower();
}
*/

Power* DevColorChangingPower::factory() {
	return new DevColorChangingPower();
}

DevColorChangingPower::DevColorChangingPower() {
	return;
}



void DevColorChangingTankPower::tick(Tank* parent) {
	Tank* closest = nullptr;
	double dist = 2*GAME_WIDTH + 2*GAME_HEIGHT;
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		if (t->getTeamID() == parent->getTeamID()) {
			continue;
		}
		double t_dist = sqrt(pow(t->x - parent->x, 2) + pow(t->y - parent->y, 2));
		if (t_dist < dist) {
			dist = t_dist;
			closest = t;
		}
	}
	if (closest == nullptr) {
		colorDist = 0;
	} else {
		colorDist = dist;
	}
	//std::cout << colorDist << std::endl;
}

ColorValueHolder DevColorChangingTankPower::getColor() const {
	if (colorDist <= 1) {
		return DevColorChangingPower::getClassColor();
	}
	double val = 1/pow(colorDist, .25); //TODO: this is bad, try something more noticeable
	return ColorValueHolder(val, val, val);
}

BulletPower* DevColorChangingTankPower::makeBulletPower() const {
	return new DevColorChangingBulletPower();
}

DevColorChangingTankPower::DevColorChangingTankPower() {
	maxTime = 500;
	timeLeft = 500;

	colorDist = 0;
}



void DevColorChangingBulletPower::tick(Bullet* parent) {
	Tank* closest = nullptr;
	double dist = 2*GAME_WIDTH + 2*GAME_HEIGHT;
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		if (t->getTeamID() == parent->getTeamID()) {
			continue;
		}
		double t_dist = sqrt(pow(t->x - parent->x, 2) + pow(t->y - parent->y, 2));
		if (t_dist < dist) {
			dist = t_dist;
			closest = t;
		}
	}
	if (closest == nullptr) {
		colorDist = 0;
	} else {
		colorDist = dist;
	}
}

ColorValueHolder DevColorChangingBulletPower::getColor() const {
	if (colorDist <= 1) {
		return DevColorChangingPower::getClassColor();
	}
	double val = 1/pow(colorDist, .25);
	return ColorValueHolder(val, val, val);
}

TankPower* DevColorChangingBulletPower::makeTankPower() const {
	return new DevColorChangingTankPower();
}

DevColorChangingBulletPower::DevColorChangingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	colorDist = 0;
}
