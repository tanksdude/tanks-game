#include "inversion-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> InversionPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* InversionPower::makeTankPower() const {
	return new InversionTankPower();
}

BulletPower* InversionPower::makeBulletPower() const {
	return new InversionBulletPower();
}

Power* InversionPower::factory() {
	return new InversionPower();
}

InversionPower::InversionPower() {
	return;
}



BulletPower* InversionTankPower::makeBulletPower() const {
	return new InversionBulletPower();
}

InversionTankPower::InversionTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;

	tankTurningIncrementStacks = true;
}



TankPower* InversionBulletPower::makeTankPower() const {
	return new InversionTankPower();
}

InversionBulletPower::InversionBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
