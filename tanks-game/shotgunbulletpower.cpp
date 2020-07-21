#include "shotgunbulletpower.h"
#include "shotguntankpower.h"

void ShotgunBulletPower::initialize(Bullet* parent) {
	//nothing
}

void ShotgunBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* ShotgunBulletPower::makeTankPower() {
	return new ShotgunTankPower();
}

ShotgunBulletPower::ShotgunBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
