#include "invinciblenamedbulletpower.h"
#include "invinciblenamedtankpower.h"

void InvincibleNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void InvincibleNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* InvincibleNamedBulletPower::makeTankPower() const {
	return new InvincibleNamedTankPower();
}

InvincibleNamedBulletPower::InvincibleNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
