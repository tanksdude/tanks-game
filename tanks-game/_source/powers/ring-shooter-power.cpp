#include "ring-shooter-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> RingShooterPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* RingShooterPower::makeTankPower() const {
	return new RingShooterTankPower();
}

BulletPower* RingShooterPower::makeBulletPower() const {
	return new RingShooterBulletPower();
}

Power* RingShooterPower::factory() {
	return new RingShooterPower();
}

RingShooterPower::RingShooterPower() {
	return;
}



#include "../constants.h"

BulletPower* RingShooterTankPower::makeBulletPower() const {
	return new RingShooterBulletPower();
}

std::vector<std::pair<float, float>>* RingShooterTankPower::addExtraShootingPoints() const {
	//see PowerFunctionHelper::equallySpacedCannonPoints
	std::vector<std::pair<float, float>>* newExtraCannonPoints = new std::vector<std::pair<float, float>>;
	newExtraCannonPoints->reserve(RingShooterPower::bulletCount-1);
	for (int i = 1; i < RingShooterPower::bulletCount; i++) {
		newExtraCannonPoints->push_back({ float(2*PI) * (i / float(RingShooterPower::bulletCount)), float(2*PI) * (-i / float(RingShooterPower::bulletCount)) });
	}
	return newExtraCannonPoints;
}

RingShooterTankPower::RingShooterTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;

	addsExtraShootingPoints = true;
}



TankPower* RingShooterBulletPower::makeTankPower() const {
	return new RingShooterTankPower();
}

RingShooterBulletPower::RingShooterBulletPower() {
	//nothing
}
