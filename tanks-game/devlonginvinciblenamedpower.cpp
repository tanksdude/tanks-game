#include "devlonginvinciblenamedpower.h"
#include "devlonginvinciblenamedtankpower.h"
#include "devlonginvinciblenamedbulletpower.h"

TankPower* DevLongInvincibleNamedPower::makeTankPower() const {
	return new DevLongInvincibleNamedTankPower();
}

BulletPower* DevLongInvincibleNamedPower::makeBulletPower() const {
	return new DevLongInvincibleNamedBulletPower();
}

/*
HazardPower* DevLongInvincibleNamedPower::makeHazardPower() const {
	return new DevLongInvincibleNamedHazardPower();
}
*/

Power* DevLongInvincibleNamedPower::factory() {
	return new DevLongInvincibleNamedPower();
}

DevLongInvincibleNamedPower::DevLongInvincibleNamedPower() {
	return;
}
