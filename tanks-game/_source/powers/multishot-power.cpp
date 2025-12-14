#include "multishot-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> MultishotPower::getWeights() const {
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

TankPower* MultishotPower::makeTankPower() const {
	return new MultishotTankPower();
}

BulletPower* MultishotPower::makeBulletPower() const {
	return new MultishotBulletPower();
}

Power* MultishotPower::factory() {
	return new MultishotPower();
}

MultishotPower::MultishotPower() {
	return;
}



#include "../power-function-helper.h"

BulletPower* MultishotTankPower::makeBulletPower() const {
	return new MultishotBulletPower();
}

std::vector<float>* MultishotTankPower::addShootingPoints() const {
	return PowerFunctionHelper::equallySpacedCannonPoints(MultishotPower::bulletCount);
}

MultishotTankPower::MultishotTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
	//JS: maxTime = 1000

	addsShootingPoints = true;
}



TankPower* MultishotBulletPower::makeTankPower() const {
	return new MultishotTankPower();
}

MultishotBulletPower::MultishotBulletPower() {
	//nothing
}
