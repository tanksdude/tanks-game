#include "devlonginvinciblenamedtankpower.h"
#include "devlonginvinciblenamedbulletpower.h"

void DevLongInvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void DevLongInvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* DevLongInvincibleNamedTankPower::makeBulletPower() const {
	return new DevLongInvincibleNamedBulletPower();
}

DevLongInvincibleNamedTankPower::DevLongInvincibleNamedTankPower() {
	maxTime = -1;
	timeLeft = 0;
}
