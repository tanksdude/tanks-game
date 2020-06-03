#pragma once
#include "multishotbulletpower.h"
#include "multishottankpower.h"
#include "multishotpower.h"

void MultishotBulletPower::initialize(Bullet*) { return; }
void MultishotBulletPower::removeEffects(Bullet*) { return; }

TankPower* MultishotBulletPower::makeTankPower() {
	return new MultishotTankPower();
}

MultishotBulletPower::MultishotBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
