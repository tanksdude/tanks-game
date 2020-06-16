#include "bananapower.h"
#include "bananatankpower.h"
#include "bananabulletpower.h"

TankPower* BananaPower::makeTankPower() {
	return new BananaTankPower();
}

BulletPower* BananaPower::makeBulletPower() {
	return new BananaBulletPower();
}

/*
HazardPower* BananaPower::makeHazardPower(){
	return new BananaHazardPower();
}
*/

Power* BananaPower::factory() {
	return new BananaPower();
}

BananaPower::BananaPower() {
	return;
}
