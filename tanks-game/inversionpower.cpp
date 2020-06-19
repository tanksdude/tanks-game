#include "inversionpower.h"
#include "inversiontankpower.h"
#include "inversionbulletpower.h"

TankPower* InversionPower::makeTankPower() {
	return new InversionTankPower();
}

BulletPower* InversionPower::makeBulletPower() {
	return new InversionBulletPower();
}

/*
HazardPower* InversionPower::makeHazardPower(){
	return new InversionHazardPower();
}
*/

Power* InversionPower::factory() {
	return new InversionPower();
}

InversionPower::InversionPower() {
	return;
}
