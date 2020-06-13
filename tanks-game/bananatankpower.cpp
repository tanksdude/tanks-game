#include "bananatankpower.h"
#include "bananabulletpower.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>

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
