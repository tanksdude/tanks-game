#include "minespower.h"
#include "minestankpower.h"
#include "minesbulletpower.h"

TankPower* MinesPower::makeTankPower() const {
	return new MinesTankPower();
}

BulletPower* MinesPower::makeBulletPower() const {
	return new MinesBulletPower();
}

/*
HazardPower* MinesPower::makeHazardPower() const {
	return new MinesHazardPower();
}
*/

Power* MinesPower::factory() {
	return new MinesPower();
}

MinesPower::MinesPower() {
	return;
}
