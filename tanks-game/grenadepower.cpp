#include "grenadepower.h"
#include "grenadetankpower.h"
#include "grenadebulletpower.h"

TankPower* GrenadePower::makeTankPower() const {
	return new GrenadeTankPower();
}

BulletPower* GrenadePower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

/*
HazardPower* GrenadePower::makeHazardPower() const {
	return new GrenadeHazardPower();
}
*/

Power* GrenadePower::factory() {
	return new GrenadePower();
}

GrenadePower::GrenadePower() {
	return;
}
