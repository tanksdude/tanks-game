#pragma once
#include "devlonginvinciblenamedbulletpower.h"
#include "devlonginvinciblenamedtankpower.h"

void DevLongInvincibleNamedBulletPower::initialize(Bullet* b) {
	return;
}

void DevLongInvincibleNamedBulletPower::removeEffects(Bullet * b) {
	return;
}

TankPower* DevLongInvincibleNamedBulletPower::makeTankPower() {
	return new DevLongInvincibleNamedTankPower();
}

DevLongInvincibleNamedBulletPower::DevLongInvincibleNamedBulletPower(){
	timeLeft = 0;
	maxTime = -1;
}