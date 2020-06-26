#include "firenamedpower.h"
#include "firenamedtankpower.h"
#include "firenamedbulletpower.h"

TankPower* FireNamedPower::makeTankPower() {
	return new FireNamedTankPower();
}

BulletPower* FireNamedPower::makeBulletPower() {
	return new FireNamedBulletPower();
}

/*
HazardPower* FireNamedPower::makeHazardPower(){
	return new FireNamedHazardPower();
}
*/

Power* FireNamedPower::factory() {
	return new FireNamedPower();
}

FireNamedPower::FireNamedPower() {
	return;
}
