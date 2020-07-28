#include "bananapower.h"
#include "bananatankpower.h"
#include "bananabulletpower.h"

TankPower* BananaPower::makeTankPower() const {
	return new BananaTankPower();
}

BulletPower* BananaPower::makeBulletPower() const {
	return new BananaBulletPower();
}

/*
HazardPower* BananaPower::makeHazardPower() const {
	return new BananaHazardPower();
}
*/

Power* BananaPower::factory() {
	return new BananaPower();
}

BananaPower::BananaPower() {
	return;
}
