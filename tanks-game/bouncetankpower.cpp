#include "bouncetankpower.h"
#include "bouncebulletpower.h"

//not reducing size on the bullet or tank in this version because I originally only did that to make it different from others
//(I think that was literally the only reason)

void BounceTankPower::initialize(Tank* parent) {
	//nothing
}

void BounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BounceTankPower::makeBulletPower() {
	return new BounceBulletPower();
}

BounceTankPower::BounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
