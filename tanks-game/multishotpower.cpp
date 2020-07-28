#include "multishotpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"

TankPower* MultishotPower::makeTankPower() const {
	return new MultishotTankPower();
}

BulletPower* MultishotPower::makeBulletPower() const {
	return new MultishotBulletPower();
}

/*
HazardPower* MultishotPower::makeHazardPower() const {
	return new MultishotHazardPower();
}
*/

Power* MultishotPower::factory() {
	return new MultishotPower();
}

MultishotPower::MultishotPower() {
	return;
}
