#include "old-mines-power.h"

std::unordered_map<std::string, float> OldMinesPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", 0.25f });
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



BulletPower* OldMinesTankPower::makeBulletPower() const {
	return new OldMinesBulletPower();
}

OldMinesTankPower::OldMinesTankPower() : MinesTankPower() {
	tankFiringRateStacks = true;
}



TankPower* OldMinesBulletPower::makeTankPower() const {
	return new OldMinesTankPower();
}

OldMinesBulletPower::OldMinesBulletPower() : MinesBulletPower() {
	//nothing new
}
