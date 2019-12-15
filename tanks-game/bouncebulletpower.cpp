#pragma once
#include "bouncebulletpower.h"
#include "bouncetankpower.h"
#include "bouncepower.h"
#include "powerfunctionhelper.h"

typedef bool (BounceBulletPower::*junkFuncForBounceBulletPower)(Bullet*, Wall*);

const short BounceBulletPower::maxBounces = 16;

bool BounceBulletPower::dummyBounceFunction(Bullet* b, Wall* w, BounceBulletPower* bp) {
	return (PowerFunctionHelper::bounceGeneric(b, w));
}

bool BounceBulletPower::dummyIntermediateBounceFunction(Bullet* b, Wall* w) {
	if (BounceBulletPower::dummyBounceFunction(b, w, this)) {
		this->bouncesLeft--;
	}

	return (this->bouncesLeft < 0);
}

void BounceBulletPower::initialize(Bullet*) { return; }
void BounceBulletPower::removeEffects(Bullet*) { return; }

TankPower* BounceBulletPower::makeTankPower() {
	return new BounceTankPower();
}

BounceBulletPower::BounceBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = BounceBulletPower::maxBounces;

	modifiedCollisionWithWall = nullptr; //dummyIntermediateBounceFunction;
}