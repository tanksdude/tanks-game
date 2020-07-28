#include "megadeathpower.h"
#include "megadeathtankpower.h"
#include "megadeathbulletpower.h"

std::unordered_map<std::string, float> MegaDeathPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .25f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "random", .25f });
	return weights;
}

TankPower* MegaDeathPower::makeTankPower() const {
	return new MegaDeathTankPower();
}

BulletPower* MegaDeathPower::makeBulletPower() const {
	return new MegaDeathBulletPower();
}

/*
HazardPower* MegaDeathPower::makeHazardPower() const {
	return new MegaDeathHazardPower();
}
*/

Power* MegaDeathPower::factory() {
	return new MegaDeathPower();
}

MegaDeathPower::MegaDeathPower() {
	return;
}
