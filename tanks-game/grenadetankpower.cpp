#pragma once
#include "grenadetankpower.h"
#include "grenadebulletpower.h"
#include "grenadepower.h"

void GrenadeTankPower::initialize(Tank* parent) {
	//nothing
}
void GrenadeTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* GrenadeTankPower::makeBulletPower() {
	return new GrenadeBulletPower();
}

GrenadeTankPower::GrenadeTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
