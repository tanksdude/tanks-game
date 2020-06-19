#include "inversionbulletpower.h"
#include "inversiontankpower.h"

void InversionBulletPower::initialize(Bullet* b) {
	//nothing
}

void InversionBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* InversionBulletPower::makeTankPower() {
	return new InversionTankPower();
}

InversionBulletPower::InversionBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
