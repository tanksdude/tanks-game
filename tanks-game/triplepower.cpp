#include "triplepower.h"
#include "tripletankpower.h"
#include "triplebulletpower.h"

TankPower* TriplePower::makeTankPower() const {
	return new TripleTankPower();
}

BulletPower* TriplePower::makeBulletPower() const {
	return new TripleBulletPower();
}

/*
HazardPower* TriplePower::makeHazardPower() const {
	return new TripleHazardPower();
}
*/

Power* TriplePower::factory() {
	return new TriplePower();
}

TriplePower::TriplePower() {
	return;
}
