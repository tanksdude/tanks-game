#include "bouncetankpower.h"
#include "bouncebulletpower.h"

//not reducing size on the bullet or tank this time because I only did that originally to make it different from others
//(I think that was literally the only reason)

void BounceTankPower::initialize(Tank* parent) {
	//parent->r /= 2;
}
void BounceTankPower::removeEffects(Tank* parent) {
	//parent->r *= 2;
}

BulletPower* BounceTankPower::makeBulletPower() {
	return new BounceBulletPower();
}

BounceTankPower::BounceTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
