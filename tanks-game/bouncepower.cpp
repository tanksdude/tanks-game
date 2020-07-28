#include "bouncepower.h"
#include "bouncetankpower.h"
#include "bouncebulletpower.h"

TankPower* BouncePower::makeTankPower() const {
	return new BounceTankPower();
}

BulletPower* BouncePower::makeBulletPower() const {
	return new BounceBulletPower();
}

/*
HazardPower* BouncePower::makeHazardPower() const {
	return new BounceHazardPower();
}
*/

Power* BouncePower::factory() {
	return new BouncePower();
}

BouncePower::BouncePower() {
	return;
}
