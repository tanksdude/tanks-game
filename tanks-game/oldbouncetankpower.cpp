#include "oldbouncetankpower.h"
#include "oldbouncebulletpower.h"

void OldBounceTankPower::initialize(Tank* parent) {
	//parent->r /= 2;
	//no need!
}
void OldBounceTankPower::removeEffects(Tank* parent) {
	//parent->r *= 2;
}

BulletPower* OldBounceTankPower::makeBulletPower() {
	return new OldBounceBulletPower();
}

OldBounceTankPower::OldBounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
