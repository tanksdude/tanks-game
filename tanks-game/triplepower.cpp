#include "triplepower.h"
#include "tripletankpower.h"
#include "triplebulletpower.h"

TankPower* TriplePower::makeTankPower() {
	return new TripleTankPower();
}

BulletPower* TriplePower::makeBulletPower() {
	return new TripleBulletPower();
}

/*
HazardPower* TriplePower::makeHazardPower(){
	return new TripleHazardPower();
}
*/

Power* TriplePower::factory() {
	return new TriplePower();
}

TriplePower::TriplePower() {
	return;
}
