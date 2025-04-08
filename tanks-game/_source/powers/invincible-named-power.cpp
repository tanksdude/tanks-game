#include "invincible-named-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> InvincibleNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f }); //TODO: something like "reduced random-vanilla" to exclude speed, invincible, wallhack
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 0.5f });
	weights.insert({ "random", 0.5f });
	return weights;
}

TankPower* InvincibleNamedPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

BulletPower* InvincibleNamedPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

Power* InvincibleNamedPower::factory() {
	return new InvincibleNamedPower();
}

InvincibleNamedPower::InvincibleNamedPower() {
	return;
}



BulletPower* InvincibleNamedTankPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

InvincibleNamedTankPower::InvincibleNamedTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
}



TankPower* InvincibleNamedBulletPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

InvincibleNamedBulletPower::InvincibleNamedBulletPower() {
	//nothing
}
