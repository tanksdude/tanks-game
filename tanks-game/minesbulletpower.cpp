#include "minesbulletpower.h"
#include "minestankpower.h"

void MinesBulletPower::initialize(Bullet* parent) {
	//nothing
}

void MinesBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* MinesBulletPower::makeTankPower() {
	return new MinesTankPower();
}

MinesBulletPower::MinesBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
