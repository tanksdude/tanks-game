#include "bananatankpower.h"
#include "bananabulletpower.h"

void BananaTankPower::initialize(Tank* parent) {
	//nothing
}
void BananaTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* BananaTankPower::makeBulletPower() {
	return new BananaBulletPower();
}

BananaTankPower::BananaTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
