#pragma once
#include "megadeathtankpower.h"
#include "megadeathbulletpower.h"
#include "megadeathpower.h"

void MegaDeathTankPower::initialize(Tank* parent) {
	//nothing
}
void MegaDeathTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* MegaDeathTankPower::makeBulletPower() {
	return new MegaDeathBulletPower();
}

MegaDeathTankPower::MegaDeathTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
