#include "multishotpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"

TankPower* MultishotPower::makeTankPower() {
	return new MultishotTankPower();
}

BulletPower* MultishotPower::makeBulletPower() {
	return new MultishotBulletPower();
}

/*
HazardPower* MultishotPower::makeHazardPower(){
	return new MultishotHazardPower();
}
*/

Power* MultishotPower::factory() {
	return new MultishotPower();
}

MultishotPower::MultishotPower() {
	return;
}
