#pragma once
class BounceTankPower;

#include "bouncetankpower.h"
#include "bouncebulletpower.h"
#include "bouncepower.h"

void BounceTankPower::initialize(Tank* parent) {
	//parent->r /= 2;
}
void BounceTankPower::removeEffects(Tank* parent) {
	//parent->r *= 2;
}

BulletPower* BounceTankPower::makeBulletPower() {
	return new BounceBulletPower();
}

BounceTankPower::BounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
