#include "megadeathpower.h"
#include "megadeathtankpower.h"
#include "megadeathbulletpower.h"

std::unordered_map<std::string, float> MegaDeathPower::getWeights() {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .25f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "random", .25f });
	return weights;
}

TankPower* MegaDeathPower::makeTankPower() {
	return new MegaDeathTankPower();
}

BulletPower* MegaDeathPower::makeBulletPower() {
	return new MegaDeathBulletPower();
}

/*
HazardPower* MegaDeathPower::makeHazardPower(){
	return new MegaDeathHazardPower();
}
*/

Power* MegaDeathPower::factory() {
	return new MegaDeathPower();
}

MegaDeathPower::MegaDeathPower() {
	return;
}
