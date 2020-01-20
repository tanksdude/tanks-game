#pragma once
class BounceTankPower;

#include "bouncetankpower.h"
#include "bouncebulletpower.h"
#include "bouncepower.h"

//not reducing size on the bullet or tank this time because I only did that originally to make it different from others
//(I think that was literally the only reason)

void BounceTankPower::initialize(Tank* parent) {
	//parent->r /= 2;
}
void BounceTankPower::removeEffects(Tank* parent) {
	//parent->r *= 2;
}

double BounceTankPower::getBulletSpeedMultiplier() { return .5; }
//double BounceTankPower::getBulletRadiusMultiplier() { return .5; } //set back to 1 if parent's radius doesn't decrease

BulletPower* BounceTankPower::makeBulletPower() {
	return new BounceBulletPower();
}

BounceTankPower::BounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
