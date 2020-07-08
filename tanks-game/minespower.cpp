#include "minespower.h"
#include "minestankpower.h"
#include "minesbulletpower.h"

TankPower* MinesPower::makeTankPower() {
	return new MinesTankPower();
}

BulletPower* MinesPower::makeBulletPower() {
	return new MinesBulletPower();
}

/*
HazardPower* MinesPower::makeHazardPower(){
	return new MinesHazardPower();
}
*/

Power* MinesPower::factory() {
	return new MinesPower();
}

MinesPower::MinesPower() {
	return;
}
