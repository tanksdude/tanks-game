#include "oldbouncepower.h"
#include "oldbouncetankpower.h"
#include "oldbouncebulletpower.h"

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
	return;
}
