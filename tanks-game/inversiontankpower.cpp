#include "inversiontankpower.h"
#include "inversionbulletpower.h"

void InversionTankPower::initialize(Tank* parent) {
	//parent->turningIncrement *= -1;
}

void InversionTankPower::removeEffects(Tank* parent) {
	//parent->turningIncrement /= -1;
}

BulletPower* InversionTankPower::makeBulletPower() {
	return new InversionBulletPower();
}

InversionTankPower::InversionTankPower() {
	maxTime = 500;
	timeLeft = 500;

	tankTurningIncrementStacks = true;
}
