#pragma once
#include "triplebulletpower.h"
#include "tripletankpower.h"
#include "triplepower.h"

void TripleBulletPower::initialize(Bullet* b) {
	//nothing
}

void TripleBulletPower::removeEffects(Bullet * b) {
	//nothing
}

TankPower* TripleBulletPower::makeTankPower() {
	return new TripleTankPower();
}

TripleBulletPower::TripleBulletPower(){
	timeLeft = 0;
	maxTime = -1;
}