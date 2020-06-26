#include "triplebulletpower.h"
#include "tripletankpower.h"

void TripleBulletPower::initialize(Bullet* parent) {
	//nothing
}

void TripleBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* TripleBulletPower::makeTankPower() {
	return new TripleTankPower();
}

TripleBulletPower::TripleBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
