#include "inversiontankpower.h"
#include "inversionbulletpower.h"

void InversionTankPower::initialize(Tank* parent) {
	//should be changed later
	parent->turningIncrement *= -1;
}
void InversionTankPower::removeEffects(Tank* parent) {
	//should be changed later
	parent->turningIncrement /= -1;
}

BulletPower* InversionTankPower::makeBulletPower() {
	return new InversionBulletPower();
}

InversionTankPower::InversionTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
