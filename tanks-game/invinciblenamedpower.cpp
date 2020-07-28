#include "invinciblenamedpower.h"
#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"

TankPower* InvincibleNamedPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

BulletPower* InvincibleNamedPower::makeBulletPower() const {
	return new InvincibleNamedBulletPower();
}

/*
HazardPower* InvincibleNamedPower::makeHazardPower() const {
	return new InvincibleNamedHazardPower();
}
*/

Power* InvincibleNamedPower::factory() {
	return new InvincibleNamedPower();
}

InvincibleNamedPower::InvincibleNamedPower() {
	return;
}
