#include "devlonginvinciblenamedbulletpower.h"
#include "devlonginvinciblenamedtankpower.h"

void DevLongInvincibleNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevLongInvincibleNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevLongInvincibleNamedBulletPower::makeTankPower() const {
	return new DevLongInvincibleNamedTankPower();
}

DevLongInvincibleNamedBulletPower::DevLongInvincibleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
