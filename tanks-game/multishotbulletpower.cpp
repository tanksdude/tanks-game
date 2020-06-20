#include "multishotbulletpower.h"
#include "multishottankpower.h"

void MultishotBulletPower::initialize(Bullet* parent) {
	//nothing
}

void MultishotBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* MultishotBulletPower::makeTankPower() {
	return new MultishotTankPower();
}

MultishotBulletPower::MultishotBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
