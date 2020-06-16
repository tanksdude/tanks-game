#include "oldbouncepower.h"
#include "oldbouncetankpower.h"
#include "oldbouncebulletpower.h"

const bool OldBouncePower::canBeInARandomLevel = true;

TankPower* OldBouncePower::makeTankPower() {
	return new OldBounceTankPower();
}

BulletPower* OldBouncePower::makeBulletPower() {
	return new OldBounceBulletPower();
}

/*
HazardPower* OldBouncePower::makeHazardPower(){
	return new OldBounceHazardPower();
}
*/

Power* OldBouncePower::factory() {
	return new OldBouncePower();
}

OldBouncePower::OldBouncePower() {
	//nope
}

OldBouncePower::~OldBouncePower() {
	//unlikely to ever need these
}
