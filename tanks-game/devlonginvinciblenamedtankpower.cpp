#include "devlonginvinciblenamedtankpower.h"
#include "devlonginvinciblenamedbulletpower.h"

void DevLongInvincibleNamedTankPower::initialize(Tank* parent) {
	return;
}
void DevLongInvincibleNamedTankPower::removeEffects(Tank* parent) {
	return;
}

BulletPower* DevLongInvincibleNamedTankPower::makeBulletPower() {
	return new DevLongInvincibleNamedBulletPower();
}

DevLongInvincibleNamedTankPower::DevLongInvincibleNamedTankPower() {
	maxTime = -1;
	timeLeft = 0;
}
