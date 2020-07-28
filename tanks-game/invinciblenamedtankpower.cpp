#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"

void InvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void InvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* InvincibleNamedTankPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

InvincibleNamedTankPower::InvincibleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
