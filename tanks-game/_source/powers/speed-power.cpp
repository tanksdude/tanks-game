#include "speed-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> SpeedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f }); //TODO: something like "reduced random-vanilla" to exclude speed, invincible, wallhack
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f }); //TODO: reduce?
	return weights;
}

TankPower* SpeedPower::makeTankPower() const {
	return new SpeedTankPower();
}

BulletPower* SpeedPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

Power* SpeedPower::factory() {
	return new SpeedPower();
}

SpeedPower::SpeedPower() {
	return;
}



BulletPower* SpeedTankPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

SpeedTankPower::SpeedTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;

	tankMaxSpeedStacks = true;
}



TankPower* SpeedBulletPower::makeTankPower() const {
	return new SpeedTankPower();
}

SpeedBulletPower::SpeedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
