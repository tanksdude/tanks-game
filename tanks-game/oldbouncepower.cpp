#include "oldbouncepower.h"
#include "oldbouncetankpower.h"
#include "oldbouncebulletpower.h"

TankPower* OldBouncePower::makeTankPower() const {
	return new OldBounceTankPower();
}

BulletPower* OldBouncePower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

/*
HazardPower* OldBouncePower::makeHazardPower() const {
	return new OldBounceHazardPower();
}
*/

Power* OldBouncePower::factory() {
	return new OldBouncePower();
}

OldBouncePower::OldBouncePower() {
	return;
}
