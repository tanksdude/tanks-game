#include "triplebulletpower.h"
#include "tripletankpower.h"

void TripleBulletPower::initialize(Bullet* parent) {
	//nothing
}

void TripleBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* TripleBulletPower::makeTankPower() const {
	return new TripleTankPower();
}

TripleBulletPower::TripleBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
