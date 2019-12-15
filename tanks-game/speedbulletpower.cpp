#pragma once
#include "speedbulletpower.h"
#include "speedtankpower.h"
#include "speedpower.h"

void SpeedBulletPower::initialize(Bullet*) { return; }
void SpeedBulletPower::removeEffects(Bullet*) { return; }

TankPower* SpeedBulletPower::makeTankPower() {
	return new SpeedTankPower();
}

SpeedBulletPower::SpeedBulletPower(){
	timeLeft = 0;
	maxTime = -1;
}