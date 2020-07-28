#include "speedbulletpower.h"
#include "speedtankpower.h"

void SpeedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void SpeedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* SpeedBulletPower::makeTankPower() const {
	return new SpeedTankPower();
}

SpeedBulletPower::SpeedBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
