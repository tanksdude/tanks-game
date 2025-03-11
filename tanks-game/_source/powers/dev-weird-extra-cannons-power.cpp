#include "dev-weird-extra-cannons-power.h"
#include "../game-manager.h" //settings

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

Power* DevWeirdExtraCannonsPower::factory() {
	return new DevWeirdExtraCannonsPower();
}

DevWeirdExtraCannonsPower::DevWeirdExtraCannonsPower() {
	return;
}



BulletPower* DevWeirdExtraCannonsTankPower::makeBulletPower() const {
	return new DevWeirdExtraCannonsBulletPower();
}

std::vector<double>* DevWeirdExtraCannonsTankPower::addShootingPoints() const {
	std::vector<double>* newCannonPoints = new std::vector<double>;
	newCannonPoints->push_back(1.0/4); //90deg
	newCannonPoints->push_back(5.0/8); //180+45deg
	return newCannonPoints;
}

DevWeirdExtraCannonsTankPower::DevWeirdExtraCannonsTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;

	addsShootingPoints = true;
}



TankPower* DevWeirdExtraCannonsBulletPower::makeTankPower() const {
	return new DevWeirdExtraCannonsTankPower();
}

DevWeirdExtraCannonsBulletPower::DevWeirdExtraCannonsBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
