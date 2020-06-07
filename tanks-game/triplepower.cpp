#include "triplepower.h"
#include "tripletankpower.h"
#include "triplebulletpower.h"
#include "constants.h"

const bool TriplePower::canBeInARandomLevel = true;

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
	//nothing
}

TriplePower::~TriplePower() {
	//unlikely to ever need these
}
