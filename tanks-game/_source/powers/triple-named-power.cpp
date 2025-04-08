#include "triple-named-power.h"
#include "../game-manager.h" //settings

#include "../constants.h"

const float TripleNamedPower::bulletAngleDiff = PI/16; //JS: PI/8

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

Power* TripleNamedPower::factory() {
	return new TripleNamedPower();
}

TripleNamedPower::TripleNamedPower() {
	return;
}



std::vector<std::pair<float, float>>* TripleNamedTankPower::addExtraShootingPoints() const {
	return new std::vector<std::pair<float, float>>{ {TripleNamedPower::bulletAngleDiff, -TripleNamedPower::bulletAngleDiff},
	                                                 {-TripleNamedPower::bulletAngleDiff, TripleNamedPower::bulletAngleDiff} };
}

BulletPower* TripleNamedTankPower::makeBulletPower() const {
	return new TripleNamedBulletPower();
}

TripleNamedTankPower::TripleNamedTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
	//JS: maxTime = 1000
	//in JS, the tank's shooting cooldown was reset

	addsExtraShootingPoints = true;
}



TankPower* TripleNamedBulletPower::makeTankPower() const {
	return new TripleNamedTankPower();
}

TripleNamedBulletPower::TripleNamedBulletPower() {
	//nothing
}
