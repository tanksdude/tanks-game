#pragma once
#include "bouncebulletpower.h"
#include "bouncetankpower.h"
#include "bouncepower.h"
#include "powerfunctionhelper.h"

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

void BounceBulletPower::initialize(Bullet* b) {
	b->velocity /= 2;
}

void BounceBulletPower::removeEffects(Bullet * b) {
	b->velocity *= 2;
}

TankPower* BounceBulletPower::makeTankPower() {
	return new BounceTankPower();
}

BounceBulletPower::BounceBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = BounceBulletPower::maxBounces;

	//bool (*tempFunc)(Bullet*, Wall*) = BounceBulletPower::dummyIntermediateBounceFunction;
	modifiedCollisionWithWall = PowerFunctionHelper::bounceGeneric;
}