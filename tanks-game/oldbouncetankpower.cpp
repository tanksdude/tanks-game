#include "oldbouncetankpower.h"
#include "oldbouncebulletpower.h"

void OldBounceTankPower::initialize(Tank* parent) {
	//nothing
}

void OldBounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldBounceTankPower::makeBulletPower() {
	return new OldBounceBulletPower();
}

OldBounceTankPower::OldBounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
