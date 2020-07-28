#include "inversionpower.h"
#include "inversiontankpower.h"
#include "inversionbulletpower.h"

TankPower* InversionPower::makeTankPower() const {
	return new InversionTankPower();
}

BulletPower* InversionPower::makeBulletPower() const {
	return new InversionBulletPower();
}

/*
HazardPower* InversionPower::makeHazardPower() const {
	return new InversionHazardPower();
}
*/

Power* InversionPower::factory() {
	return new InversionPower();
}

InversionPower::InversionPower() {
	return;
}
