#pragma once
#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"
#include "invinciblenamedpower.h"

void InvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
}
void InvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* InvincibleNamedTankPower::makeBulletPower() {
	return new InvincibleNamedBulletPower();
}

InvincibleNamedTankPower::InvincibleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
