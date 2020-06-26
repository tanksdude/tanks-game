#include "inversionbulletpower.h"
#include "inversiontankpower.h"

void InversionBulletPower::initialize(Bullet* parent) {
	//nothing
}

void InversionBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* InversionBulletPower::makeTankPower() {
	return new InversionTankPower();
}

InversionBulletPower::InversionBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
