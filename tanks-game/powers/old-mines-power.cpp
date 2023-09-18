#include "old-mines-power.h"

std::unordered_map<std::string, float> OldMinesPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .25f });
	return weights;
}

TankPower* OldMinesPower::makeTankPower() const {
	return new OldMinesTankPower();
}

BulletPower* OldMinesPower::makeBulletPower() const {
	return new OldMinesBulletPower();
}

/*
HazardPower* OldMinesPower::makeHazardPower() const {
	return new OldMinesHazardPower();
}
*/

Power* OldMinesPower::factory() {
	return new OldMinesPower();
}

OldMinesPower::OldMinesPower() : MinesPower() {
	return;
}



void OldMinesTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void OldMinesTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldMinesTankPower::makeBulletPower() const {
	return new OldMinesBulletPower();
}

OldMinesTankPower::OldMinesTankPower() : MinesTankPower() {
	tankFiringRateStacks = true;
}



void OldMinesBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldMinesBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* OldMinesBulletPower::makeTankPower() const {
	return new OldMinesTankPower();
}

OldMinesBulletPower::OldMinesBulletPower() : MinesBulletPower() {
	//nothing new
}
